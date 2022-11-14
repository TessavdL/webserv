/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_server.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/23 13:39:17 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/14 12:36:38 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>

#include "../includes/webserv.hpp"
#include "../includes/event_loop/connection.hpp"
#include "../includes/virtual_server/select_virtual_server.hpp"
#include "../includes/virtual_server/select_location.hpp"
#include "../includes/http_response/error_checking.hpp"

#define BUFF_SIZE 4096
#define MAX_EVENTS 100

#define EVENT_FD event[i].ident
#define EVENT_FLAGS event[i].flags
#define EVENT_FILTER event[i].filter

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

void	create_sockets_with_config(vector<Server>	server, map<int, vector<Server> > &sockets_with_config)
{
	map<int, vector<Server> >	ports_with_config;

	for (vector<Server>::iterator it = server.begin(); it != server.end(); ++it) {
		for (vector<pair<string, int> >::const_iterator it2 = (*it).get_host_and_port().begin(); it2 != (*it).get_host_and_port().end(); ++it2) {
			ports_with_config[(*it2).second] = vector<Server>();
		}
	}
	for (map<int, vector<Server> >::iterator it = ports_with_config.begin(); it != ports_with_config.end(); ++it) {
		for (vector<Server>::iterator it2 = server.begin(); it2 != server.end(); ++it2) {
			for (vector<pair<string, int> >::const_iterator it3 = (*it2).get_host_and_port().begin(); it3 != (*it2).get_host_and_port().end(); ++it3) {
				if ((*it).first == (*it3).second)
					(*it).second.push_back((*it2));
			}
		}
	}
	for (map<int, vector<Server> >::iterator it = ports_with_config.begin(); it != ports_with_config.end(); ++it) {
		SocketListen	socket((*it).first);
		sockets_with_config[socket.get_fd()] = (*it).second;
	}
}

bool	client_disconnected(u_short event_flag) {
	if (event_flag & EV_EOF) {
		return (true);
	}
	return (false);
}

bool	is_readable_event(short event_filter) {
	if (event_filter == EVFILT_READ) {
		return (true);
	}
	return (false);
}

bool	is_writable_event(short event_filter) {
	if (event_filter == EVFILT_WRITE) {
		return (true);
	}
	return (false);
}

bool	is_new_connection(int event_identifier, map<int, vector<Server> > sockets_with_config) {
	map<int, vector<Server> >::const_iterator it = sockets_with_config.find(event_identifier);
	if (it != sockets_with_config.end()) {
		return (true);
	}
	return (false);
}

int	accept_connection(int event_fd) {
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int connection_fd = accept(event_fd, (struct sockaddr*)&client_addr, &client_len);

	if (connection_fd == -1) {
		error_and_exit("accept");
	}
	return (connection_fd);
}

void	add_connection(int event_fd, int connection_fd, map<int, Connection>& connections, map<int, vector<Server> > virtual_servers) {
	pair<int, Connection>	new_connection;
	map<int, vector<Server> >::const_iterator it = virtual_servers.find(event_fd);
	new_connection.first = connection_fd;
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

void	add_event_to_kqueue(int kq, int event_fd, int event_filter) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, event_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		error_and_exit("kevent");
	}
}

bool	identify_client(int event_identifier, map<int, Connection> connections) {
	map<int, Connection>::iterator it = connections.find(event_identifier);
	if (it != connections.end()) {
		return (true);
	}
	return (false);
}

void	save_request(Connection& client, HTTPRequestLexer lexer, int bytes_in_data, int total_bytes_read) {
	Connection::t_request	request;

	request.request_line.method = lexer.get_request_line_method();
	request.request_line.uri = lexer.get_request_line_uri();
	request.request_line.protocol = lexer.get_request_line_protocol();
	request.headers = lexer.get_headers();
	request.body = lexer.get_body();
	request.bytes_in_data = bytes_in_data;
	request.total_bytes_read = total_bytes_read;
	client.set_request(request);
}

void	receive_request_from_client(int connection_fd, Connection& client, int bytes_in_data) {
	HTTPRequestLexer		lexer;
	long					total_bytes_read = 0;
	int						bytes_read = 1;
	char					buf[BUFF_SIZE];

	cout << "--- reading from client socket ---" << endl;
	while (bytes_read > 0) {
		bytes_read = recv(connection_fd, buf, sizeof(buf), 0);
		if (bytes_read == -1) {
			break ;
		}
		if (bytes_read > 0 && bytes_read < BUFF_SIZE) {
			cout << "--- finished reading from socket ---" << endl;
		}
		buf[bytes_read] = '\0';
		lexer.process_request(string(buf));
		if (lexer.get_state() == HTTPRequestLexer::REQUEST_ERROR) {
			cout << "--- request is invalid ---" << endl;
			break ;
		}
		total_bytes_read += bytes_read;
	}
	save_request(client, lexer, bytes_in_data, total_bytes_read);
	printf("--- done reading ---\n");
}

void	send_request_to_client(int connection_fd, Connection& client) {
	client.set_server_index(select_virtual_server(client.get_request().request_line.uri.get_authority_host(), client.get_virtual_servers().second));
	// std::cout << "virtual server index = " << client.get_server_index() << std::endl;
	
	client.set_location_index(select_location(client.get_request().request_line.uri.get_path_full(), client.get_virtual_servers().second[client.get_server_index()].get_location_block()));
	// std::cout << "location index = " << client.get_location_index() << std::endl;
	
	pair<int, string> status = initial_error_checking(client, client.get_request());
	std::cout << "status_code = " << status.first << " reason_phrase = " << status.second << std::endl;
	std::string response = client.get_response().get_full_response();
	const char *buf = response.c_str();
	send(connection_fd, buf, strlen(buf), 0);
	printf("--- done writing to client socket\n");
	close(connection_fd);
	printf("--- bounce bye ---\n\n");
}

int kqueue_server(vector<Server> server)
{
	map<int, vector<Server> >	sockets_with_config;
	map<int, Connection>		connections;

	create_sockets_with_config(server, sockets_with_config);

	//	CREATE KERNEL QUEUE
    int	kq = kqueue();
	if (kq == -1) {
		return (error_and_exit("An error occured in kqueue().\n"));
	}

    // Create event 'filter', these are the events we want to monitor.
    // Here we want to monitor: socket_listen_fd, for the events: EVFILT_READ 
    // (when there is data to be read on the socket), and perform the following
    // actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue 
    // and enable it).
	for (map<int, vector<Server> >::iterator it = sockets_with_config.begin(); it != sockets_with_config.end(); it++) {
		add_event_to_kqueue(kq, (*it).first, EVFILT_READ);
	}

    // REGISTER SOCKET FD TO KERNEL QUEUE
    // if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
	// 	return (error_and_exit("An error occured in kevent() while trying to register the kernel event to the queue.\n"));
    // }

    // EVENT LOOP
    for (;;)
    {
		struct kevent	event[MAX_EVENTS];
        // CHECK FOR NEW EVENTS
		// Do not register new events with the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
        // Only handle 1 new event per iteration in the loop; 5th argument is 1.
        int n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
        if (n_events == -1) {
			return (error_and_exit("An error occured in kevent() while checking for new events.\n"));
        }

		// LOOP OVER n_events
        for (int i = 0; n_events > i; i++)
        {
            // When the client disconnects an EOF is sent. By closing the file
            // descriptor the event is automatically removed from the kqueue.
<<<<<<< HEAD
            if (event[i].flags & EV_EOF) {
                close(event_fd);
=======
            if (client_disconnected(EVENT_FLAGS)) {
                printf("--- a client has disconnected ---\n");
                close(EVENT_FD);
				connections.erase(EVENT_FD);
>>>>>>> main
				// do not close socket_connection_fd, is bad file descriptor
            }

            // If the new event's file descriptor is the same as the listening
            // socket's file descriptor, we are sure that a new client wants 
            // to connect to our socket.
<<<<<<< HEAD
            else if (match_event(event_fd, sockets_with_config) == true)
            {
				// Incoming socket connection on the listening socket.
				// Create a new socket for the actual connection to client.
				socket_connection_fd = accept(event_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_len);
				if (socket_connection_fd == -1) {
					return (error_and_exit("An error occured in accept()\n"));
				}

				// Put this new socket connection also as a 'filter' event
				// to watch in kqueue, so we can now watch for events on this
				// new socket.
				EV_SET(&change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
                    return (error_and_exit("An error occured in kevent() when registering connected socket to the queue\n"));
				}
			}

			// READY TO READ FROM CLIENT SOCKET
			else if (event[i].filter == EVFILT_READ)
			{
				HTTPRequestLexer	lexer;
				long				total_amount_of_bytes_read = 0;
				int					bytes_read = 1;
				char				buf[BUFF_SIZE];
	
				while (bytes_read > 0) {
					bytes_read = recv(event_fd, buf, sizeof(buf), 0);
					if (bytes_read == -1) {
						break ;
					}
					lexer.process_request(std::string(buf));
					if (lexer.get_state() == HTTPRequestLexer::REQUEST_START || lexer.get_state() == HTTPRequestLexer::REQUEST_ERROR) {
						break ;
					}
					total_amount_of_bytes_read += bytes_read;
				}
				
				EV_SET(&change_event, socket_connection_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
				if (kevent(kq, &change_event, 1, NULL, 0, NULL) == -1) {
					return (error_and_exit("An error occured in kevent() when registering write event to the queue\n"));
				}
			}
			else if (event[i].filter == EVFILT_WRITE) {
				Response	response;
				const char *buf = response.get_full_response().c_str();
				// std::cout << std::endl << response << std::endl;

				send(event_fd, buf, strlen(buf), 0);
				close(event_fd);
=======
			else if (is_new_connection(EVENT_FD, sockets_with_config) == true) {
				int connection_fd = accept_connection(EVENT_FD);
				add_event_to_kqueue(kq, connection_fd, EVFILT_READ);
				add_connection(EVENT_FD, connection_fd, connections, sockets_with_config);
			}

			// READY TO READ FROM CLIENT SOCKET
			else if (is_readable_event(EVENT_FILTER) == true) {
				if (identify_client(EVENT_FD, connections) == true) {
					Connection& client = connections[EVENT_FD];
					receive_request_from_client(EVENT_FD, client, event[i].data);
					client.print_request();
					add_event_to_kqueue(kq, EVENT_FD, EVFILT_WRITE);
				}
			}

			// READY TO WRITE TO CLIENT SOCKET
			else if (is_writable_event(EVENT_FILTER) == true) {
				printf("--- writing to client socket ---\n");
				if (identify_client(EVENT_FD, connections) == true) {
					Connection& client = connections[EVENT_FD];
					send_request_to_client(EVENT_FD, client);
					connections.erase(EVENT_FD);
				}
>>>>>>> main
			}
		}
	}
	return (0);
}
