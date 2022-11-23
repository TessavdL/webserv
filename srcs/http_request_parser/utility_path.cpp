/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_path.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:29:58 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 13:00:36 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/utility_path.hpp"

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

bool	file_exists(const char *str) {
	struct stat stat_buf;

	if (stat(str, &stat_buf) == 0) {
		return (true);
	}
	return (false);
}

bool	is_regular_file_stat(const char* str) {
	struct stat	stat_buf;

	if (stat(str, &stat_buf) == -1) {
		return (false);
	}
	return (S_ISREG(stat_buf.st_mode));
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

std::string	remove_multiple_consecutive_characters(std::string const& str, char c) {
	std::string	ret;

	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] == c && str[i + 1] && str[i + 1] == c) {}
		else {
			ret.push_back(str[i]);
		}
	}
	return (ret);
}

std::string	root_plus_uri_path(std::string const& root, std::string const& uri_path) {
	return (remove_multiple_consecutive_characters(root + uri_path,'/'));
}
