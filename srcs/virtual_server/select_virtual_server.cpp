/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_virtual_server.cpp                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 13:43:20 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 14:51:05 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/select_virtual_server.hpp"

static size_t	longest_match(std::string str1, std::string str2) {
	size_t	i = 0;
	for (;str1[i] && str2[i] && str1[i] == str2[i]; i++) {}
	return (i);
}

size_t	select_virtual_server_index(std::string const& host, std::vector<Server> const& v) {
	size_t		l = 0;
	size_t		max = 0;
	size_t		virtual_server_index = 0;

	if (host.empty())
		return (virtual_server_index);
	for (size_t i = 0; i < v.size(); i++) {
		for (size_t j = 0; i < v[i].get_server_name().size(); j++) {
			l = longest_match(host, v[i].get_server_name()[j]);
			if (max < l) {
				max = l;
				virtual_server_index = i;
			}
		}
	}
	return (virtual_server_index);
}

// int main(int argc, char **argv) {
// 	std::vector<t_virtual_server> v;

// 	t_virtual_server	one;
// 	t_virtual_server	two;
// 	t_virtual_server	three;
	
// 	one.server_name = "example.com";
// 	two.server_name = "example.net";
// 	three.server_name = "example.org";

// 	v.push_back(one);
// 	v.push_back(two);
// 	v.push_back(three);

// 	for (std::vector<t_virtual_server>::iterator it = v.begin(); it != v.end(); it++) {
// 		std::cout << (*it).server_name << std::endl;
// 	}

// 	std::vector<std::string>	host;
// 	for (int i = 1; i < argc; i++) {
// 		host.push_back(argv[i]);
// 	}

// 	for (std::vector<std::string>::iterator it = host.begin(); it != host.end(); it++) {
// 		std::string	longest_match;
// 		longest_match = v[select_virtual_server(*it, v)].server_name;
// 		std::cout << "longest match for " << *it << " is " << longest_match << std::endl;
// 	}
// 	return (0);
// }
