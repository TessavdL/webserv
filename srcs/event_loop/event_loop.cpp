/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/07 22:29:12 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 15:54:05 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"

void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config) {
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

static void	reset_response_data(Connection& client) {
	ResponseData	response_data;
	
	client.response = response_data;
}

void	send_response_to_client(std::map<int, Connection>& connections, int const kq, struct kevent& event) {
	Connection&	client = connections[event.ident];
	std::string	response_string = client.response.get_repsonse_string();
	const char	*buf = response_string.c_str();
	ssize_t		size = response_string.size();
	ssize_t		bytes_sent = 0;
	ssize_t		total_bytes_sent = client.response.get_total_bytes_sent();

	bytes_sent = send(event.ident, buf + total_bytes_sent, size - total_bytes_sent, 0);
	client.response.set_total_bytes_sent(total_bytes_sent + bytes_sent);
	if ((bytes_sent == 0 || client.response.get_total_bytes_sent() == size) && !connection_is_continue(client.response.get_headers())) {
		close(event.ident);
		connections.erase(event.ident);
		std::cout << "closed event identifier [" << event.ident << "]\n";
	}
	else if ((bytes_sent == 0 || client.response.get_total_bytes_sent() == size) && connection_is_continue(client.response.get_headers())) {
		delete_write_event_from_kqueue(kq, &event, event.ident);
		add_read_event_to_kqueue(kq, client.get_connection_fd());
		reset_response_data(client);
	}
}

int event_loop(vector<Server> server) {
	int							kq;
	map<int, vector<Server> >	listening_sockets_with_config;
	map<int, Connection>		connections;

	create_listening_sockets_with_config(server, listening_sockets_with_config);
	new_kernel_event_queue(kq);
	register_listening_sockets_to_kernel_events_kqueue(kq, listening_sockets_with_config);

	for (;;) {
		struct kevent	event[MAX_EVENTS];
		int				n_events = 0;
		
		check_for_hanging_connections(connections, kq);
		n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
		if (n_events == -1) {
			throw (FatalException("SYSCALL: kevent in event_loop\n"));
		}
		for (int i = 0; n_events > i; i++) {
			std::cout << "\nevent identifier [" << event[i].ident << "]\n";
			if (is_event_error(event[i].flags)) {
				throw (FatalException("KEVENT EV_ERROR\n"));
			}
			else if (client_disconnected(event[i].flags)) {
				std::cout << "client [" << event[i].ident << "] has disconnected\n";
				close(event[i].ident);
				connections.erase(event[i].ident);
			}
			else if (is_new_connection(event[i].ident, listening_sockets_with_config)) {
				int connection_fd = accept_connection(event[i].ident);
				std::cout << "client [" << connection_fd << "] has connected\n";
				add_read_event_to_kqueue(kq, connection_fd);
				add_connection(connections, listening_sockets_with_config, event[i].ident, connection_fd);
			}
			else if (is_readable_event(event[i].filter)) {
				std::cout << "readable event [" << event[i].ident << "]\n\n";
				if (is_client(connections, event[i].ident)) {
					Connection& client = connections[event[i].ident];
					handle_request(client, kq, event[i]);
				}
			}
			else if (is_writable_event(event[i].filter)) {
				std::cout << "writable event [" << event[i].ident << "]\n\n";
				if (is_client(connections, event[i].ident)) {
					Connection& client = connections[event[i].ident];
					if (!response_is_generated(client.response.get_generated())) {
						prepare_response(client);
						client.response.set_generated(true);
					}
					send_response_to_client(connections, kq, event[i]);
				}
			}
		}
	}
	return (0);
}
