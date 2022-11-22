/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_for_file.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 13:27:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/22 18:06:53 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEARCH_FOR_FILE_HPP
# define SEARCH_FOR_FILE_HPP

#include <string>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "../fatal_exception.hpp"
#include "../http_request_parser/utility_path.hpp"
#include "../event_loop/connection.hpp"
#include "../http_response/error_checking.hpp"
#include "../http_response/error_page_string.hpp"

std::string						search_for_file_in_dir(std::vector<std::string>	const& v, std::string const& directory);
std::string						create_current_working_directory(void);
std::pair<std::string, bool>	handle_file_location(std::string const& path);
std::pair<std::string, bool>	search_for_file_to_serve(std::vector<std::string> const& index, std::string const path);
std::string						create_path(std::string const& root, std::string const& uri_path);
std::string						default_error_page_content(void);
std::string 					default_error_page_location(void);
std::string						find_error_page_location(int& status_code, VirtualServer const& virtual_server);

#endif
