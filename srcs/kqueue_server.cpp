/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_server.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/23 13:39:17 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/12 12:30:14 by jelvan-d      ########   odam.nl         */
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
#include "../includes/http_response/error_checking.hpp"
#include "../includes/http_response/response_handler.hpp"

#define BUFF_SIZE 4096
#define MAX_EVENTS 100
#define EVENT_FD event[i].ident
#define EVENT_FLAGS event[i].flags
#define EVENT_FILTER event[i].filter

void	create_sockets_with_config(vector<Server> server, map<int, vector<Server> >& sockets_with_config)
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

bool	is_event_error(u_short event_flag) {
	if (event_flag & EV_ERROR) {
		return (true);
	}
	return (false);
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
	int 				connection_fd = accept(event_fd, (struct sockaddr*)&client_addr, &client_len);

	if (connection_fd == -1) {
		throw (FatalException("SYSCALL: accept in accept_connection\n"));
	}
	return (connection_fd);
}

void	add_connection(int event_fd, int connection_fd, map<int, Connection>& connections, map<int, vector<Server> > virtual_servers) {
	pair<int, Connection>	new_connection;
	map<int, vector<Server> >::const_iterator it = virtual_servers.find(event_fd);
	new_connection.first = connection_fd;
	new_connection.second.set_connection_fd(connection_fd);
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

void	add_event_to_kqueue(int kq, int event_fd, int event_filter) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, event_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

bool	identify_client(int event_identifier, map<int, Connection> connections) {
	map<int, Connection>::iterator it = connections.find(event_identifier);

	if (it != connections.end()) {
		return (true);
	}
	return (false);
}

int	prepare_error_response_to_client(Connection& client, int const status_code) {
	ResponseData	response_data;

	response_data.set_status_code(status_code);
	client.set_response(response_data);
	return (-1);
}

static bool	ready_to_check_request_line_and_headers(RequestHandler::State state) {
	if (state >= RequestHandler::REQUEST_CHECK) {
		return (true);
	}
	return (false);
}

void	save_request(Connection& client, RequestHandler parser, int bytes_in_data, int total_bytes_read) {
	RequestData	request;
	
	request.set_bytes_in_data(bytes_in_data);
	request.set_total_bytes_read(total_bytes_read);
	request.set_method(parser.get_request_line_method());
	request.set_uri(parser.get_request_line_uri());
	request.set_protocol(parser.get_request_line_protocol());
	request.set_headers(parser.get_headers());
	request.set_body(parser.get_body());
	client.set_request(request);
}

void	save_request_line_and_headers(Connection& client, RequestHandler parser) {
	RequestData request;
	
	request.set_method(parser.get_request_line_method());
	request.set_uri(parser.get_request_line_uri());
	request.set_protocol(parser.get_request_line_protocol());
	request.set_headers(parser.get_headers());
	client.set_request(request);
}

int	parse_received_data(Connection& client, RequestHandler& parser, char buf[BUFF_SIZE + 1]) {
	try {
		parser.process_request(string(buf));
	}
	catch (RequestException const& e) {
		return (prepare_error_response_to_client(client, e.get_status_code()));
	}
	if (ready_to_check_request_line_and_headers(parser.get_state())) {
		save_request_line_and_headers(client, parser);
		client.select_virtual_server();
		try {
			error_check_request_line_and_headers(client, client.get_request());
		}
		catch (RequestException const& e2) {
			return (prepare_error_response_to_client(client, e2.get_status_code()));
		}
	}
	return (0);
}

void	receive_request_from_client(int connection_fd, Connection& client, int bytes_in_data) {
	RequestHandler	parser;
	long			total_bytes_read = 0;
	int				bytes_read = 1;
	char			buf[BUFF_SIZE + 1];

	cout << "--- start reading from client ---" << endl;
	while (bytes_read > 0) {
		bytes_read = recv(connection_fd, buf, BUFF_SIZE, 0);
		if (bytes_read == -1) {
			break ;
		}
		buf[bytes_read] = '\0';
		total_bytes_read += bytes_read;
		if (parse_received_data(client, parser, buf) == -1) {
			printf("--- finished reading from client ---\n");
			return ;
		}
	}
	save_request(client, parser, bytes_in_data, total_bytes_read);
	printf("--- finished reading from client ---\n");
}

void	send_response_to_client(int connection_fd, Connection& client) {
	ResponseHandler		response_handler;
	response_handler.handle_response(client);

	ResponseGenerator	response;
	response.generate_response_string(client.get_response());

	string				response_string = response.get_full_response();
	unsigned long		size = response_string.size();
	const char			*buf = response_string.c_str();

	cout << response;
	send(connection_fd, buf, size, 0);
	printf("--- finished writing to client\n");
	close(connection_fd);
	printf("--- bounce client, bye! ---\n\n");
}

int kqueue_server(vector<Server> server) {
	map<int, vector<Server> >	sockets_with_config;
	map<int, Connection>		connections;

	create_sockets_with_config(server, sockets_with_config);

	//	CREATE KERNEL QUEUE
    int	kq = kqueue();
	if (kq == -1) {
		throw (FatalException("SYSCALL: kq in kqueue_server\n"));
	}

    // Create event 'filter', these are the events we want to monitor.
    // Here we want to monitor: socket_listen_fd, for the events: EVFILT_READ 
    // (when there is data to be read on the socket), and perform the following
    // actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue 
    // and enable it).
	for (map<int, vector<Server> >::iterator it = sockets_with_config.begin(); it != sockets_with_config.end(); it++) {
		add_event_to_kqueue(kq, (*it).first, EVFILT_READ);
	}

    // EVENT LOOP
    for (;;)
    {
		struct kevent	event[MAX_EVENTS];
        // CHECK FOR NEW EVENTS
		// Do not register new events with the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
        // Only handle 1 new event per iteration in the loop; 5th argument is 1.
        int n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
        if (n_events == -1) {
			throw (FatalException("SYSCALL: kevent in kqueue_server\n"));
        }

		// LOOP OVER n_events
        for (int i = 0; n_events > i; i++)
        {
			// An error has occured while processing the events
            if (is_event_error(EVENT_FLAGS)) {
				throw (FatalException("KEVENT EV_ERROR\n"));
			}
			
			 // When the client disconnects an EOF is sent. By closing the file
            // descriptor the event is automatically removed from the kqueue.
			else if (client_disconnected(EVENT_FLAGS)) {
                printf("--- client has disconnected ---\n");
                close(EVENT_FD);
				connections.erase(EVENT_FD);
            }

            // If the new event's file descriptor is the same as the listening
            // socket's file descriptor, we are sure that a new client wants 
            // to connect to our socket.
			else if (is_new_connection(EVENT_FD, sockets_with_config)) {
				int connection_fd = accept_connection(EVENT_FD);
				add_event_to_kqueue(kq, connection_fd, EVFILT_READ);
				add_connection(EVENT_FD, connection_fd, connections, sockets_with_config);
			}

			// READY TO READ FROM CLIENT SOCKET
			else if (is_readable_event(EVENT_FILTER)) {
				if (identify_client(EVENT_FD, connections) == true) {
					Connection& client = connections[EVENT_FD];
					receive_request_from_client(EVENT_FD, client, event[i].data);
					client.print_request();
					add_event_to_kqueue(kq, EVENT_FD, EVFILT_WRITE);
				}
			}

			// READY TO WRITE TO CLIENT SOCKET
			else if (is_writable_event(EVENT_FILTER)) {
				printf("--- writing to client ---\n");
				if (identify_client(EVENT_FD, connections) == true) {
					Connection& client = connections[EVENT_FD];
					send_response_to_client(EVENT_FD, client);
					connections.erase(EVENT_FD);
				}
			}
		}
	}
	return (0);
}
