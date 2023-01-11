/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop_handle_new_connection.cpp               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 16:14:17 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:13:13 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/event_loop_actions.hpp"

static int	accept_connection(int const event_fd) {
	struct sockaddr_in	client_addr;
	socklen_t			client_len = sizeof(client_addr);
	int 				connection_fd = accept(event_fd, (struct sockaddr*)&client_addr, &client_len);

	if (connection_fd == -1) {
		throw (FatalException("SYSCALL: accept in accept_connection\n"));
	}
	return (connection_fd);
}

static void	add_connection(std::map<int, Connection>& connections, std::map<int, std::vector<Server> > const& virtual_servers, int const event_fd, int const connection_fd) {
	pair<int, Connection>						new_connection;
	map<int, vector<Server> >::const_iterator	it = virtual_servers.find(event_fd);

	new_connection.first = connection_fd;
	new_connection.second.set_time();
	new_connection.second.set_connection_fd(connection_fd);
	new_connection.second.set_virtual_servers(*it);
	connections.insert(new_connection);
}

void	handle_new_connection(std::map<int, Connection>& connections, std::map<int, std::vector<Server> > const& listening_sockets_with_config, int const kq, int const event_fd) {
	int connection_fd = accept_connection(event_fd);

	std::cout << "client [" << connection_fd << "] has connected\n\n";
	add_read_event_to_kqueue(kq, connection_fd);
	add_connection(connections, listening_sockets_with_config, event_fd, connection_fd);
}
