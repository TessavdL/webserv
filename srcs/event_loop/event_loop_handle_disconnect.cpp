/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop_handle_disconnect.cpp                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 16:08:51 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 16:13:35 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/event_loop_actions.hpp"

void	handle_disconnect(std::map<int, Connection>& connections, int const event_fd) {
	std::cout << "client [" << event_fd << "] has disconnected\n";
	close(event_fd);
	connections.erase(event_fd);
}
