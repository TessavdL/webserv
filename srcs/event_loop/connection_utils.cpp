/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection_utils.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 14:53:01 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 15:11:28 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/connection_utils.hpp"

int	accept_connection(int const event_fd) {
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int 				connection_fd = accept(event_fd, (struct sockaddr*)&client_addr, &client_len);

	if (connection_fd == -1) {
		throw (FatalException("SYSCALL: accept in accept_connection\n"));
	}
	return (connection_fd);
}

void	add_connection(std::map<int, Connection>& connections, std::map<int, std::vector<Server> > const& virtual_servers, int const event_fd, int const connection_fd) {
	pair<int, Connection>						new_connection;
	map<int, vector<Server> >::const_iterator	it = virtual_servers.find(event_fd);

	new_connection.first = connection_fd;
	new_connection.second.set_time();
	new_connection.second.set_connection_fd(connection_fd);
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

void	check_for_hanging_connections(std::map<int, Connection>& connections, int const kq) {
	std::vector<int>	hanging_connections;
	double				second_since_start = 0.0;

	for (std::map<int, Connection>::const_iterator it = connections.cbegin(); it != connections.cend(); it++) {
		second_since_start = difftime(time(0), it->second.get_time());
		if (second_since_start > 4.0) {
			hanging_connections.push_back(it->first);
		}
	}
	for (std::vector<int>::const_iterator it = hanging_connections.cbegin(); it != hanging_connections.cend(); it++) {
		connections[*it].response.set_status_code(408);
		add_write_event_to_kqueue(kq, *it);
	}
}

bool	is_client(std::map<int, Connection> const& connections, int const event_fd) {
	std::map<int, Connection>::const_iterator	it = connections.find(event_fd);

	if (it != connections.end()) {
		return (true);
	}
	return (false);
}
