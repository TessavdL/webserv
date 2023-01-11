/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_path.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 17:34:26 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:35:32 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_PATH_HPP
# define UTILITY_PATH_HPP

# include <dirent.h>
# include <string>
# include <sys/stat.h>
# include <unistd.h>
# include <vector>

bool						is_absolute_path(const char* str);
bool						is_directory(const char* str);
bool						is_directory_stat(const char* str);
bool						file_exists(const char *str);
bool						is_regular_file_stat(const char* str);
std::vector<std::string>	get_directory_file_list(std::string const& filename);
bool						has_read_permission(const char* str);
bool						has_write_permission(const char* str);
bool						has_execute_permission(const char* str);
std::string					remove_multiple_consecutive_characters(std::string const& str, char c);
std::string					root_plus_uri_path(std::string const& uri_path, std::string const& root);

#endif
