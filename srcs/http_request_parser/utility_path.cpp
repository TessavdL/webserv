/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_path.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:29:58 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/27 17:05:43 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

bool	is_absolute_path(const char* str) {
	if (str[0] && str[0] == '/') {
			return (true);
	}
	return (false);
}

bool	is_directory(const char* str) {
	if (str[0] && str[strlen(str) - 1] == '/') {
		return (true);
	}
	return (false);
}

bool	is_directory_stat(const char* str) {
	struct stat	stat_buf;

	if (stat(str, &stat_buf) == -1) {
		return (false);
	}
	return (S_ISDIR(stat_buf.st_mode));
	
}

bool	is_regular_file_stat(const char* str) {
	struct stat	stat_buf;

	if (stat(str, &stat_buf) == -1) {
		return (false);
	}
	return (S_ISREG(stat_buf.st_mode));
}

std::string	remove_consequetive_characters(std::string const& str, char c)
{
	std::string	ret;

	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == c && str[i + 1] && str[i + 1] == c) {}
		else {
			ret.push_back(str[i]);
		}
	}
	return (ret);
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

bool	has_read_permission(const char* str) {
	if (access(str, R_OK) == -1) {
		return (false);
	}
	return (true);
}

bool	has_write_permission(const char* str) {
	if (access(str, W_OK) == -1) {
		return (false);
	}
	return (true);
}

bool	has_execute_permission(const char* str) {
	if (access(str, X_OK) == -1) {
		return (false);
	}
	return (true);
}

std::string		root_plus_uri_path(std::string const& uri_path, std::string const& root) {
	return(remove_consequetive_characters(root + uri_path,'/'));
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
	std::cout << std::boolalpha << "tempdir " << is_directory_stat("tempdir") << std::endl;
	std::cout << std::boolalpha << "a.out " << is_regular_file_stat("a.out") << std::endl;
	std::cout << std::boolalpha << "test " << is_regular_file_stat("test") << std::endl;
	std::cout << std::boolalpha << "a.out " << has_read_permission("a.out") << std::endl;
	std::cout << std::boolalpha << "a.out " << has_write_permission("a.out") << std::endl;
	std::cout << std::boolalpha << "a.out " << has_execute_permission("a.out") << std::endl;
	std::cout << std::boolalpha << "test " << has_read_permission("test") << std::endl;
	std::cout << std::boolalpha << "test " << has_write_permission("test") << std::endl;
	std::cout << std::boolalpha << "test " << has_execute_permission("test") << std::endl;
	std::cout << root_plus_uri_path(std::string("/index.html"), std::string("var/www/html/"));
	return (0);
}
