/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_server.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/23 13:39:17 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/24 12:17:49 by jelvan-d      ########   odam.nl         */
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
// #define event_fd event[i].ident
// #define EVENT_FLAGS event[i].flags
// #define EVENT_FILTER event[i].filter

void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config)
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
		listening_sockets_with_config[socket.get_fd()] = (*it).second;
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

bool	event_identifier_equals_listening_socket_fd(int event_identifier, map<int, vector<Server> > listening_sockets_with_config) {
	map<int, vector<Server> >::const_iterator it = listening_sockets_with_config.find(event_identifier);

	if (it != listening_sockets_with_config.end()) {
		return (true);
	}
	return (false);
}

bool	is_new_connection(int event_identifier, map<int, vector<Server> > listening_sockets_with_config) {
	if (event_identifier_equals_listening_socket_fd(event_identifier, listening_sockets_with_config)) {
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
	pair<int, Connection>						new_connection;
	map<int, vector<Server> >::const_iterator	it = virtual_servers.find(event_fd);

	new_connection.first = connection_fd;
	new_connection.second.set_connection_fd(connection_fd);
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

void	add_write_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	add_new_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	add_read_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	std::cout << "ADDING READ EVENT TO KQUEUE = " << event_fd << std::endl;
	EV_SET(&monitor_event, event_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	register_listening_sockets_to_kernel_events_kqueue(int const kq, map<int, vector<Server> > listening_sockets_with_config) {
	for (map<int, vector<Server> >::iterator it = listening_sockets_with_config.begin(); it != listening_sockets_with_config.end(); it++) {
		add_new_event_to_kqueue(kq, (*it).first);
	}
}

void	new_kernel_event_queue(int& kq) {
	kq = kqueue();
	if (kq == -1) {
		throw (FatalException("SYSCALL: kq in kqueue_server\n"));
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

int	parse_received_data(Connection& client, RequestHandler& parser, std::string const& buf) {
	try {
		parser.process_request(buf);
	}
	catch (RequestException const& e) {
		std::cout << e.what() << std::endl;
		std::cout << e.get_status_code() << std::endl;
		return (prepare_error_response_to_client(client, e.get_status_code()));
	}
	if (ready_to_check_request_line_and_headers(parser.get_state())) {
		save_request_line_and_headers(client, parser);
		client.select_virtual_server();
		try {
			error_check_request_line_and_headers(client, client.get_request());
		}
		catch (RequestException const& e2) {
			std::cout << e2.what() << std::endl;
			std::cout << e2.get_status_code() << std::endl;
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

	cout << "--- start reading from client ---" << connection_fd << endl;
	while (bytes_read > 0) {
		bytes_read = recv(connection_fd, buf, BUFF_SIZE, 0);
		if (bytes_read == -1) {
			break ;
		}
		buf[bytes_read] = '\0';
		total_bytes_read += bytes_read;
		if (parse_received_data(client, parser, string(buf, bytes_read)) == -1) {
			while (bytes_read > 0) {
				bytes_read = recv(connection_fd, buf, BUFF_SIZE, 0);
				if (bytes_read == -1) {
					break ;
				}
				buf[bytes_read] = '\0';
				total_bytes_read += bytes_read;
			}
			printf("--- finished reading from client ---\n");
			return ;
		}
	}
	save_request(client, parser, bytes_in_data, total_bytes_read);
	printf("--- finished reading from client ---\n");
}

static bool	is_continue(std::map<std::string, std::string> const& headers) {
	std::map<std::string, std::string>::const_iterator it = headers.find("Connection");

	if (it != headers.end()) {
		if (!it->second.compare("Keep-Alive")) {
			return (true);
		}
		return (false);
	}
	return (false);
}

void	send_response_to_client(int connection_fd, Connection& client) {
	ResponseHandler	response_handler;
	std::string r;
	response_handler.handle_response(client);

	if (response_handler.get_status() == ResponseHandler::CGI)
		r = client.get_response().get_full_response();
	else {
		ResponseGenerator response;

		response.generate_response_string(client.get_response());
		r = response.get_full_response();
	}
	unsigned long size = r.size();
	const char *buf = r.c_str();
	// cout << buf << endl;

	send(connection_fd, buf, size, 0);
	printf("--- done writing to client socket\n");
	if (!is_continue(client.get_response().get_headers())) {
		close(connection_fd);
	}
	std::cout << "--- closed event fd = " << connection_fd << "---\n" << std::endl;
}

int kqueue_server(vector<Server> server) {
	int							kq;
	map<int, vector<Server> >	listening_sockets_with_config;
	map<int, Connection>		connections;

	create_listening_sockets_with_config(server, listening_sockets_with_config);
	new_kernel_event_queue(kq);
	register_listening_sockets_to_kernel_events_kqueue(kq, listening_sockets_with_config);

	for (;;) {
		// https://stackoverflow.com/questions/19641000/favicon-is-not-loading-in-chrome
		// remove inner for loop and check one event at the time
		struct kevent	event[MAX_EVENTS];
		int n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
		std::cout << "amount of events = " << n_events << std::endl;
		if (n_events == -1) {
			throw (FatalException("SYSCALL: kevent in kqueue_server\n"));
		}
		for (int i = 0; n_events > i; i++) {
			std::cout << "event[i].ident = " << event[i].ident << std::endl;
			std::cout << "data = " << event[i].data << std::endl;
			std::cout << "filter = " << event[i].filter << std::endl << std::endl;
			if (is_event_error(event[i].flags)) {
				throw (FatalException("KEVENT EV_ERROR\n"));
			}
			else if (client_disconnected(event[i].flags)) {
				std::cout << "--- Client " << event[i].ident << " has disconnected" << std::endl;
                close(event[i].ident);
				connections.erase(event[i].ident);
            }
			else if (is_new_connection(event[i].ident, listening_sockets_with_config)) {
				std::cout << "new connection = " << event[i].ident << std::endl;
				int connection_fd = accept_connection(event[i].ident);
				add_read_event_to_kqueue(kq, connection_fd);
				add_connection(event[i].ident, connection_fd, connections, listening_sockets_with_config);
			}
			else if (is_readable_event(event[i].filter)) {
				std::cout << "readable event = " << event[i].ident << std::endl;
				Connection& client = connections[event[i].ident];
				receive_request_from_client(event[i].ident, client, event[i].data);
				client.print_request();
				add_write_event_to_kqueue(kq, event[i].ident);
			}
			else if (is_writable_event(event[i].filter)) {
				std::cout << "writable event = " << event[i].ident << std::endl;
				Connection& client = connections[event[i].ident];
				send_response_to_client(event[i].ident, client);
				if (!is_continue(client.get_response().get_headers()))
					connections.erase(event[i].ident);
				else {
					add_read_event_to_kqueue(kq, event[i].ident);
				}
			}
		}
		usleep(400);
		std::cout << "end of event loop\n" << std::endl;
	}
	return (0);
}
