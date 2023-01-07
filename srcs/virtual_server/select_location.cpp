/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_location.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 14:20:22 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/07 21:36:30 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/select_location.hpp"

static size_t	longest_match(std::string str1, std::string str2) {
	size_t	i = 0;
	for (;str1[i] && str2[i] && str1[i] == str2[i]; i++) {}
	return (i);
}

int	select_location_index(std::string const& uri_path, std::vector<LocationBlock> const& v) {
	size_t		l = 0;
	size_t		max = 0;
	int		location_index = 0;

	if (v.empty())
		return (NO_LOCATION);
	for (size_t i = 0; i < v.size(); i++) {
		if (v[i].get_path_and_optional_modifier().size() > 1 && !v[i].get_path_and_optional_modifier()[0].compare("=")) {
			if (v[i].get_path_and_optional_modifier().back() == uri_path) {
				return (i);
			}
			else {
				continue ;
			}
		}
		l = longest_match(uri_path, v[i].get_path_and_optional_modifier().back());
		if (max < l) {
			max = l;
			location_index = i;
		}
	}
	return (location_index);
}
