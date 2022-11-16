#ifndef UTILITY_PATH_HPP
# define UTILITY_PATH_HPP

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

bool						is_absolute_path(const char* str);
bool						is_directory(const char* str);
bool						is_directory_stat(const char* str);
bool						file_exists(const char *str);
bool						is_regular_file_stat(const char* str);
std::vector<std::string>	get_directory_file_list(std::string const& filename);
bool						has_read_permission(const char* str);
bool						has_write_permission(const char* str);
bool						has_execute_permission(const char* str);
std::string					remove_consequetive_characters(std::string const& str, char c);
std::string					root_plus_uri_path(std::string const& uri_path, std::string const& root);

#endif
