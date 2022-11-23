/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_for_file.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 13:28:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 12:51:03 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/search_for_file.hpp"

static void	free_scandir_memory(dirent **name_list, int n) {
	for (int i = 0; i < n; i++) {
		free(name_list[i]);
	}
	free(name_list);
}

static bool	is_file_in_directory_entry(const char *file, dirent** name_list, int name_list_size) {
	for (int i = 0; i < name_list_size; i++) {
		if (!strcmp(file, name_list[i]->d_name)) {
			return (true);
		}
	}
	return (false);
}

std::string	search_for_file_in_dir(std::vector<std::string>	const& v, std::string const& directory) {
	std::string		file;
	struct dirent	**name_list;
	int				name_list_size;
	
	name_list_size = scandir(directory.c_str(), &name_list, NULL, alphasort);
	if (name_list_size == -1) {
		throw (FatalException("scandir"));
	}
	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); it++) {
		if (is_file_in_directory_entry((*it).c_str(), name_list, name_list_size)) {
			file = *it;
			break ;
		}
	}
	free_scandir_memory(name_list, name_list_size);
	return (file);
}

std::string	create_path(std::string const& root, std::string const& uri_path) {
	return (remove_consequetive_characters(create_current_working_directory() + "/" + root + "/" + uri_path, '/'));
}

std::string	create_current_working_directory(void) {
	char		*buf = NULL;
	char		*ptr = NULL;
	long		size;
	std::string	current_working_directory;

	size = pathconf(".", _PC_PATH_MAX);
	if ((buf = (char *)malloc((size_t)size)) != NULL) {
    	ptr = getcwd(buf, (size_t)size);
	}
	else {
		throw (FatalException("GETCWD"));
	}
	current_working_directory = std::string(buf);
	free(buf);
	return (current_working_directory);
}

std::string	default_error_page_content(void) {
	return (ERROR_PAGE_STRING);
}

std::string default_error_page_location(void) {
	return (create_current_working_directory() + "/" + "error_page.html");
}

// std::pair<std::string, bool>	handle_file_location(std::string const& path) {
// 	if (!file_exists(path.c_str())) {
// 		return (std::pair<std::string, bool>("", false));
// 	}
// 	return (std::pair<std::string, bool>(path, true));
// }

// returns true if the file is found
// returns false if the file is not found
// the file is either a direct path or it is created from a directory + index file
std::pair<std::string, bool>	search_for_file_to_serve(std::vector<std::string> const& index, std::string const path) {
	std::string	file_location = path;

	if (!file_exists(file_location.c_str())) {
		return (std::pair<std::string, bool>("", false));
	}
	if (is_directory_stat(file_location.c_str())) {
		std::string const	file_name = search_for_file_in_dir(index, file_location);
		if (!file_name.empty()) {
			return (std::pair<std::string, bool>(remove_consequetive_characters((file_location + "/" + file_name), '/'), true));
		}
		else {
			return (std::pair<std::string, bool>(file_location, false));
		}
	}
	return (std::pair<std::string, bool>(file_location, true));
}

std::string	find_error_page_location(int& status_code, VirtualServer const& virtual_server) {
	std::vector<std::pair<std::vector<int>, std::string> > const	error_page = virtual_server.get_error_page();
	std::string const												root = virtual_server.get_root();
	
	if (!error_page.empty()) {
		for (std::vector<std::pair<std::vector<int>, std::string> >::const_iterator it = error_page.begin(); it != error_page.end(); it++) {
			for (std::vector<int>::const_iterator it2 = it->first.begin(); it2 != it->first.end(); it2++) {
				if (*it2 == status_code) {
					return (it->second);
				}
			}
		}
	}
	return ("");
}
