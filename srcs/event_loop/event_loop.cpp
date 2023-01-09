/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/07 22:29:12 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 13:03:29 by tevan-de      ########   odam.nl         */
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

static void	reset_response_data(Connection& client) {
	ResponseData	response_data;
	
	client.set_response(response_data);
}

static bool	connection_is_continue(std::map<std::string, std::string> const& headers) {
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
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier def(Color::FG_DEFAULT);
	ResponseHandler	response_handler;
	std::string r;
	response_handler.handle_response(client);

	if (response_handler.get_status() == ResponseHandler::CGI)
		r = client.get_response().get_full_response();
	else {
		ResponseGenerator response;

		response.generate_response_string(client.get_response());
		std::cout << green;
		std::cout << response << def;
		r = response.get_full_response();
	}
	unsigned long size = r.size();
	const char *buf = r.c_str();

	send(connection_fd, buf, size, 0);
	if (!connection_is_continue(client.get_response().get_headers())) {
		close(connection_fd);
		std::cout << "closed event identifier [" << connection_fd << "]" << std::endl;
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
		int n_events = kevent(kq, NULL, 0, event, MAX_EVENTS, NULL);
		if (n_events == -1) {
			throw (FatalException("SYSCALL: kevent in event_loop\n"));
		}
		for (int i = 0; n_events > i; i++) {
			std::cout << "\nevent identifier [" << event[i].ident << "]" << std::endl;
			if (is_event_error(event[i].flags)) {
				throw (FatalException("KEVENT EV_ERROR\n"));
			}
			else if (client_disconnected(event[i].flags)) {
				std::cout << "client [" << event[i].ident << "] has disconnected" << std::endl;
                close(event[i].ident);
				connections.erase(event[i].ident);
            }
			else if (is_new_connection(event[i].ident, listening_sockets_with_config)) {
				int connection_fd = accept_connection(event[i].ident);
				std::cout << "client [" << connection_fd << "] has connected" << std::endl;
				add_read_event_to_kqueue(kq, connection_fd);
				add_connection(event[i].ident, connection_fd, connections, listening_sockets_with_config);
			}
			else if (is_readable_event(event[i].filter)) {
				std::cout << "readable event [" << event[i].ident << "]\n" << std::endl;
				Connection& client = connections[event[i].ident];
				receive_request(client, event[i].ident, event[i].data);
				client.print_request();
				add_write_event_to_kqueue(kq, event[i].ident);
			}
			else if (is_writable_event(event[i].filter)) {
				std::cout << "writable event [" << event[i].ident << "]\n" << std::endl;
				Connection& client = connections[event[i].ident];
				send_response_to_client(event[i].ident, client);
				if (!connection_is_continue(client.get_response().get_headers())) {
					connections.erase(event[i].ident);
				}
				else {
					delete_event_from_kqueue(kq, &event[i], event[i].ident);
					add_read_event_to_kqueue(kq, client.get_connection_fd());
					reset_response_data(client);
				}
			}
		}
	}
	return (0);
}
