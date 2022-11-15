/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_virtual_server.hpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 12:00:48 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 14:50:43 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_VIRTUAL_SERVER_HPP
# define SELECT_VIRTUAL_SERVER_HPP

#include <string>
#include <vector>

#include "../../includes/config_parser/server.hpp"

size_t	select_virtual_server_index(std::string const& host, std::vector<Server> const& v);

#endif
