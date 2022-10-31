/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_server.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/23 13:39:17 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/31 18:21:39 by tevan-de      ########   odam.nl         */
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

#define BUFF_SIZE 4096
#define MAX_EVENTS 100

// bool	match_event(int event_fd, map<int, vector<Server> > sockets_with_config)
// {
// 	cout << "event fd = " << event_fd << endl;
// 	map<int, vector<Server> >::iterator it;
// 	it = sockets_with_config.find(event_fd);
// 	if (it != sockets_with_config.end())
// 		return (true);
// 	return (false);
// }

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

pair<int, Connection>	identify_client(int event_identifier, map<int, Connection> connections) {
	map<int, Connection>::iterator it = connections.find(event_identifier);
	return (*it);
}

int kqueue_server(vector<Server>	server)
{
	map<int/*socket_fds*/, vector<Server> >	sockets_with_config;
	map<int, Connection>					connections;

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
            if (event[i].flags & EV_EOF) {
                printf("--- a client has disconnected ---\n");
                close(event[i].ident);
				connections.erase(event[i].ident);
				// do not close socket_connection_fd, is bad file descriptor
            }

            // If the new event's file descriptor is the same as the listening
            // socket's file descriptor, we are sure that a new client wants 
            // to connect to our socket.
			else if (is_new_connection(event[i].ident, sockets_with_config) == true) {
				int connection_fd = accept_connection(event[i].ident);
				add_event_to_kqueue(kq, connection_fd, EVFILT_READ);
				add_connection(event[i].ident, connection_fd, connections, sockets_with_config);
			}

			// READY TO READ FROM CLIENT SOCKET
			else if (event[i].filter == EVFILT_READ)
			{
				pair<int, Connection>	client = identify_client(event[i].ident, connections);
				HTTPRequestLexer		lexer;
				long					total_amount_of_bytes_read = 0;
				int						bytes_read = 1;
				char					buf[BUFF_SIZE];

				cout << "--- reading from client socket ---" << endl;
				cout << "--- amount of bytes in data = " << event[i].data << "---" << endl;
	
				while (bytes_read > 0) {
					bytes_read = recv(event[i].ident, buf, sizeof(buf), 0);
					if (bytes_read == -1) {
						break ;
					}
					if (bytes_read > 0 && bytes_read < BUFF_SIZE) {
						cout << "--- finished reading from socket ---" << endl;
					}
					lexer.process_request(string(buf));
					if (lexer.get_state() == HTTPRequestLexer::REQUEST_START || lexer.get_state() == HTTPRequestLexer::REQUEST_ERROR) {
						cout << "--- request is invalid ---" << endl;
						break ;
					}
					total_amount_of_bytes_read += bytes_read;
				}

				if (total_amount_of_bytes_read == event[i].data) {
					cout << "--- all data send by the socket was received ---" << endl;
				}
				else {
					cout << "--- an error occured, not all send by the socket was received ---" << endl;
				}
				add_event_to_kqueue(kq, client.first, EVFILT_WRITE);
				printf("--- done reading ---\n");
			}
			else if (event[i].filter == EVFILT_WRITE) {
				printf("--- writing to client socket ---\n");
				Response	response;
				const char *buf = response.get_full_response().c_str();
				cout << endl << response << endl;

				send(event[i].ident, buf, strlen(buf), 0);
				printf("--- done writing to client socket\n");
				close(event[i].ident);
				printf("--- bounce bye ---\n\n");
			}
		}
	}
	return (0);
}
