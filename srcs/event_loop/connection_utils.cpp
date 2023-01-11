/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection_utils.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 14:53:01 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 16:38:59 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/connection_utils.hpp"

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
