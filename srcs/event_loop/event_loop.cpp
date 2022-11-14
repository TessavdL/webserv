#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <map>
#include <vector>

#include "../../includes/listening_sockets/socket_listen.hpp"
#include "../../includes/http_request_parser/http_request_lexer.hpp"
#include "../../includes/http_request_parser/http_request_parser.hpp"
#include "../../includes/http_response/response.hpp"
#include "../../includes/event_loop/connection.hpp"

#define MAX_EVENTS 100

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

bool	is_readable_event(int event_filter) {
	if (event_filter == EVFILT_READ) {
		return (true);
	}
	return (false);
}

bool	is_writable_event(int event_filter) {
	if (event_filter == EVFILT_WRITE) {
		return (true);
	}
	return (false);
}

void	add_event_to_kqueue(int kq, int event_fd, int event_filter) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, event_filter, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		error_and_exit("kevent");
	}
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

void	add_connection(int event_fd, int connection_fd, std::map<int, Connection>& connections, std::map<int, std::vector<Server> > virtual_servers) {
	std::pair<int, Connection>	new_connection;
	std::map<int, std::vector<Server> >::const_iterator it = virtual_servers.find(event_fd);
	new_connection.first = connection_fd;
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

bool	is_new_connection(int event_identifier, std::map<int, std::vector<Server> > virtual_servers) {
	std::map<int, std::vector<Server> >::const_iterator it = virtual_servers.find(event_identifier);
	if (it != virtual_servers.end()) {
		return (true);
	}
	return (false);
}

int	main(void) {
	// initiate connection map
	std::map<int, std::vector<Server> >	virtual_servers;
	std::map<int, Connection>			connections;
	
	// create kernel kqueue
	int	kq = kqueue();
	if (kq == -1) {
		return (error_and_exit("kqueue"));
	}

	// register	socket_fds for monitoring by the kqueue
	for (std::map<int, std::vector<Server> >::iterator it = virtual_servers.begin(); it != virtual_servers.end(); it++) {
		add_event_to_kqueue(kq, (*it).first, EVFILT_READ);
	}

	// remove connections from connection map that are no longer relevant due to time out for example

	// event looop
	for (;;) {
		// poll for events
		struct kevent event[MAX_EVENTS];
		int	n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
		if (n_events == -1) {
			error_and_exit("kevent");
		}

		// loop over polled events
		for (int i = 0; i < n_events; i++) {
			// a client disconnected
			// closing the file discriptor removes event from kqueue
			// also remove from connection map
			if (event[i].flags & EV_EOF) {
				if (close(event[i].ident) == -1) {
					error_and_exit("close");
				}
			}
			// a new client is trying to connect
			else if (is_new_connection(event[i].ident, virtual_servers) == true) {
				int connection_fd = accept_connection(event[i].ident);
				add_event_to_kqueue(kq, connection_fd, EVFILT_READ);
				add_connection(event[i].ident, connection_fd, connections, virtual_servers);
			}
			// it's possible to read to the file descriptor, read the request from the client
			else if (is_readable_event(event[i].filter) == true) {
				// std::pair<int, connection> client = identify_client(event[i].ident, connection);
				// receive_request(client);
				add_event_to_kqueue(kq, /*client.first*/, EVFILT_WRITE);
			}
			// it's possible to write to the file descriptor, write the response to the client
			else if (is_writable_event(event[i].filter) == true) {
				// std::pair<int, connection> client = identify_client(event[i].ident, connection);
				// send_response(client);
				// after the request has been sent the file descriptor of the client can be closed
				// after the request has been sent the connection can be removed from the connection map
			}
		}
	}
	return (0);
}
