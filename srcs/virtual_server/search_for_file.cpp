#include <string>
#include <vector>
#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <algorithm>

#define BUFFERSIZE 1024

// std::string	read_file(std::string const& filename) {
// 	char		*buf;

// 	buf = (char *)malloc(sizeof(char) * BUFFERSIZE + 1);

// 	buf[BUFFERSIZE] = '\0';
// }

bool	is_file_in_directory_entry(const char *file, dirent** namelist) {
	for (size_t i = 0; namelist[i]; i++) {
		if (!strcmp(file, namelist[i]->d_name)) {
			return (true);
		}
	}
	return (false);
}

std::string	search_for_file_in_dir(std::vector<std::string>	const& v, std::string const& directory) {
	std::string		file;
	struct dirent	**namelist;
	int				n;			
	
	n = scandir(directory.c_str(), &namelist, NULL, alphasort);
	if (n == -1) {
		perror("scandir");
		exit(EXIT_FAILURE);
	}
	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); it++) {
		if (is_file_in_directory_entry((*it).c_str(), namelist)) {
			file = *it;
			std::cout << file << std::endl;
			break ;
		}
	}
	return (file);
}

int	main(int argc, char **argv) {
	std::vector<std::string>	v;

	for (int i = 1; i < argc; i++) {
		v.push_back(std::string(argv[i]));
	}
	std::string	index = search_for_file_in_dir(v, std::string("test_index"));
		// if (!index.empty()) {
		// 	std::string	file_contents = read_file(index);
		// 	if (!file_contents.empty()) {
		// 		std::cout << file_contents << std::endl;
		// 	}
		// }
	return (0);
}
