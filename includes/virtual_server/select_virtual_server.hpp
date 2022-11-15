/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_virtual_server.hpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 12:00:48 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 17:00:38 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_VIRTUAL_SERVER_HPP
# define SELECT_VIRTUAL_SERVER_HPP

#include <string>
#include <vector>

#include "../../includes/config_parser/server.hpp"

size_t	determine_server_index(std::string const& host, std::vector<Server> const& v);

#endif
