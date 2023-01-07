/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_location.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 14:20:22 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/07 17:04:41 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/select_location.hpp"

static size_t	longest_match(std::string str1, std::string str2) {
	size_t	i = 0;
	for (;str1[i] && str2[i] && str1[i] == str2[i]; i++) {}
	return (i);
}

// assumes that uri_path is not empty
int	select_location_index(std::string const& uri_path, std::vector<LocationBlock> const& v) {
	size_t		l = 0;
	size_t		max = 0;
	int		location_index = 0;

	// std::cout << "WHY WE CRASH?" << std::endl;
	// std::cout << "size of v = " << v.size() << std::endl;
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

// int main(int argc, char **argv) {
// 	std::vector<t_location> v;

// 	t_location	one;
// 	t_location	two;
// 	t_location	three;
	
// 	one.path = "/";
// 	one.exact_match = true;
// 	two.path = "/directory1/";
// 	two.exact_match = true;
// 	three.path = "/directory1/directory2/";
// 	three.exact_match = false;

// 	v.push_back(one);
// 	v.push_back(two);
// 	v.push_back(three);

// 	for (std::vector<t_location>::iterator it = v.begin(); it != v.end(); it++) {
// 		std::cout << (*it).path << std::endl;
// 	}

// 	std::vector<std::string>	host;
// 	for (int i = 1; i < argc; i++) {
// 		host.push_back(argv[i]);
// 	}

// 	for (std::vector<std::string>::iterator it = host.begin(); it != host.end(); it++) {
// 		std::string	longest_match;
// 		longest_match = v[select_location(*it, v)].path;
// 		std::cout << "longest match for " << *it << " is " << longest_match << std::endl;
// 	}
// 	return (0);
// }
