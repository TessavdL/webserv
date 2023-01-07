/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_virtual_server.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 13:43:20 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/07 21:36:55 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/select_virtual_server.hpp"

static size_t	longest_match(std::string str1, std::string str2) {
	size_t	i = 0;
	for (;str1[i] && str2[i] && str1[i] == str2[i]; i++) {}
	return (i);
}

size_t	select_server_index(std::string const& host, std::vector<Server> const& v) {
	size_t		l = 0;
	size_t		max = 0;
	size_t		server_index = 0;

	if (host.empty())
		return (server_index);
	for (size_t i = 0; i < v.size(); i++) {
		for (size_t j = 0; i < v[i].get_server_name().size(); j++) {
			l = longest_match(host, v[i].get_server_name()[j]);
			if (max < l) {
				max = l;
				server_index = i;
			}
		}
	}
	return (server_index);
}
