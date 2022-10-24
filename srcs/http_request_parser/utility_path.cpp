/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_path.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:29:58 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/18 17:42:19 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>

bool	is_absolute_path(std::string const& str) {
	if (!str.empty()) {
		if (str[0] == '/') {
			return (true);
		}
	}
	return (false);
}

bool	is_directory(std::string const& str) {
	if (!str.empty()) {
		if (str[str.length() - 1] == '/') {
			return (true);
		}
	}
	return (false);
}

std::vector<std::string>	get_directory_file_list(std::string const& filename) {
	DIR							*dir;
	struct dirent				*ent;
	std::vector<std::string>	v;

	dir = opendir(filename.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			v.push_back(std::string(ent->d_name));
		}
		closedir(dir);
	}
	return (v);
}

std::string		root_plus_uri_path(std::string uri_path, std::string root) {
	if (is_directory(root) == true) {
		
	}
}

int main(void) {
	std::cout << std::boolalpha << "/directory/file.html" << " " << is_absolute_path("/directory/file.html") << std::endl;
	std::cout << std::boolalpha << "directory/file.html" << " " << is_absolute_path("directory/file.html") << std::endl;
	std::cout << std::boolalpha << "file.html" << " " << is_absolute_path("file.html") << std::endl;
	std::cout << std::boolalpha << "" << " " << is_absolute_path("") << std::endl;
	std::cout << std::boolalpha << "/directory/file.html" << " " << is_directory("/directory/file.html") << std::endl;
	std::cout << std::boolalpha << "directory/file.html" << " " << is_directory("directory/file.html") << std::endl;
	std::cout << std::boolalpha << "file.html" << " " << is_directory("file.html") << std::endl;
	std::cout << std::boolalpha << "" << " " << is_directory("") << std::endl;
	std::cout << std::boolalpha << "/directory/" << " " << is_absolute_path("/directory/") << std::endl;
	std::cout << std::boolalpha << "directory/" << " " << is_absolute_path("directory/") << std::endl;
	std::cout << std::boolalpha << "/directory/directory/" << " " << is_absolute_path("/directory/directory/") << std::endl;
	std::cout << std::boolalpha << "directory/directory/file.html" << " " << is_absolute_path("directory/directory/file.html") << std::endl;
	std::vector<std::string> v = get_directory_file_list(std::string("./"));
	if (!v.empty()) {
		for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); it++) {
			std::cout << *it << std::endl;
		}
	}
}
