/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection_utils.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 15:04:51 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 15:12:27 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_UTILS_HPP
# define CONNECTION_UTILS_HPP

# include <ctime>
# include <map>
# include <vector>

# include "./connection.hpp"
# include "./kqueue_and_kevent_utils.hpp"
# include "../config_parser/server.hpp"
# include "../fatal_exception.hpp"

int		accept_connection(int const event_fd);
void	add_connection(std::map<int, Connection>& connections, std::map<int, std::vector<Server> > const& virtual_servers, int const event_fd, int const connection_fd);
void	check_for_hanging_connections(std::map<int, Connection>& connections, int const kq);
bool	is_client(std::map<int, Connection> const& connections, int const event_fd);

#endif
