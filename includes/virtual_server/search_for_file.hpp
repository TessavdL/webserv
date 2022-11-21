/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_for_file.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 13:27:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/21 15:48:03 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEARCH_FOR_FILE_HPP
# define SEARCH_FOR_FILE_HPP

#include <string>
#include <vector>
#include <dirent.h>

#include "../fatal_exception.hpp"
#include "../http_request_parser/utility_path.hpp"
#include "../event_loop/connection.hpp"
#include "../http_response/error_checking.hpp"

std::string						search_for_file_in_dir(std::vector<std::string>	const& v, std::string const& directory);
std::string						create_current_working_directory(void);
std::pair<std::string, bool>	handle_file_location(std::string const& path);
std::pair<std::string, bool>	handle_file_location(std::vector<std::string> const& index, std::string const path);
// bool							file_is_found(bool file_location);
std::string						default_error_page(void);
std::string						find_error_page_location(int& status_code, VirtualServer const& virtual_server);

#endif
