/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_location.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/24 14:20:22 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/24 17:53:16 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>

typedef struct	s_location {
	std::string	path;
	bool		exact_match;
}				t_location;

size_t	longest_match(std::string str1, std::string str2) {
	size_t	i = 0;
	for (;str1[i] && str2[i] && str1[i] == str2[i]; i++) {}
	return (i);
}

size_t	select_location(std::string uri_path, std::vector<t_location> v) {
	size_t		l = 0;
	size_t		max = 0;
	size_t		location_index = 0;

	for (size_t i = 0; i < v.size(); i++) {
		if (v[i].exact_match == true) {
			if (v[i].path == uri_path) {
				return (i);
			}
			else {
				continue ;
			}
		}
		l = longest_match(uri_path, v[i].path);
		if (max < l) {
			max = l;
			location_index = i;
		}
	}
	return (location_index);
}

int main(int argc, char **argv) {
	std::vector<t_location> v;

	t_location	one;
	t_location	two;
	t_location	three;
	
	one.path = "/";
	one.exact_match = true;
	two.path = "/directory1/";
	two.exact_match = true;
	three.path = "/directory1/directory2/";
	three.exact_match = false;

	v.push_back(one);
	v.push_back(two);
	v.push_back(three);

	for (std::vector<t_location>::iterator it = v.begin(); it != v.end(); it++) {
		std::cout << (*it).path << std::endl;
	}

	std::vector<std::string>	host;
	for (int i = 1; i < argc; i++) {
		host.push_back(argv[i]);
	}

	for (std::vector<std::string>::iterator it = host.begin(); it != host.end(); it++) {
		std::string	longest_match;
		longest_match = v[select_location(*it, v)].path;
		std::cout << "longest match for " << *it << " is " << longest_match << std::endl;
	}
	return (0);
}
