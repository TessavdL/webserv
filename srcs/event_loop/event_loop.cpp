/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/07 22:29:12 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:16:21 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/webserv.hpp"

static void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config) {
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
	std::cout << "\n";
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
			if (is_event_error(event[i].flags)) {
				throw (FatalException("KEVENT EV_ERROR\n"));
			}
			else if (client_disconnected(event[i].flags)) {
				handle_disconnect(connections, event[i].ident);
			}
			else if (is_new_connection(event[i].ident, listening_sockets_with_config)) {
				handle_new_connection(connections, listening_sockets_with_config, kq, event[i].ident);
			}
			else if (is_readable_event(event[i].filter)) {
				handle_readable_event(connections, kq, event[i]);
			}
			else if (is_writable_event(event[i].filter)) {
				handle_writable_event(connections, kq, event[i]);
			}
		}
	}
	return (0);
}
