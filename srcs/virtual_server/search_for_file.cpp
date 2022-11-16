/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_for_file.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 13:28:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/16 14:00:50 by tevan-de      ########   odam.nl         */
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
		perror("scandir");
		exit(EXIT_FAILURE);
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

// remove testing main when we start using it later
// use when path is a directory
// question: does it has to be an html file? should we check for extension?
// int	main(int argc, char **argv) {
// 	std::vector<std::string>	v;

// 	for (int i = 1; i < argc; i++) {
// 		v.push_back(std::string(argv[i]));
// 	}
// 	std::string	index = search_for_file_in_dir(v, std::string("test_index"));
// 	if (index.empty()) {
// 		std::cout << "There is no file in this directory that matches the index files from the configuration" << std::endl;
// 	}
// 	else {
// 		std::cout << index << std::endl;
// 	}
// 	return (0);
// }
