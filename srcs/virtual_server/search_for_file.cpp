#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>

void	free_scandir_memory(dirent **namelist, int n) {
	for (int i = 0; i < n; i++) {
		free(namelist[i]);
	}
	free(namelist);
}

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
			break ;
		}
	}
	free_scandir_memory(namelist, n);
	return (file);
}

int	main(int argc, char **argv) {
	std::vector<std::string>	v;

	for (int i = 1; i < argc; i++) {
		v.push_back(std::string(argv[i]));
	}
	std::string	index = search_for_file_in_dir(v, std::string("test_index"));
	if (index.empty()) {
		std::cout << "There is no file in this directory that matches the index files from the configuration" << std::endl;
	}
	else {
		std::cout << index << std::endl;
	}
	return (0);
}
