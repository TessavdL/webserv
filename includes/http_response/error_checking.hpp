/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:15 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/21 14:50:11 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CHECKING_HPP
# define ERROR_CHECKING_HPP

# include "../event_loop/connection.hpp"
# include "../virtual_server/select_location.hpp"
# include "../http_request_parser/utility_path.hpp"

# include <map>
# include <string>
# include <vector>

# define NO_CONTENT_LENGTH -2
# define INVALID_CONTENT_LENGTH -1
# define OK 0
# define KO 1

std::map<int, std::string>	create_status_code_reason_phrase_map(void);
std::string const&			get_reason_phrase(int status_code);
int							check_user_information(int& status_code, std::string const& uri_user_information);
int							check_if_complete(int& status_code, int const bytes_in_data, int const total_bytes_read);
int							check_method(int& status_code, std::string const& method, std::vector<std::string> const& allowed_methods);
int							check_request_size(int& status_code, int request_body_size, int content_length_size);
int							check_uri_length(int& status_code, std::string const& request_uri);
int							check_http_protocol(int& status_code, std::string const& protocol);
int							check_if_file_is_processable(int& status_code, std::string const& file_location);
int							check_if_file_has_read_permission(int& status_code, std::string const& file_location);
bool						check_if_file_is_found(int& status_code, bool file_location);

int							initial_error_checking(int& status_code, Connection& client, Connection::t_request const& request);
int							check_file_status(int& status_code, std::string const& file);

#endif
