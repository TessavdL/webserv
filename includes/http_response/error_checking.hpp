/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:15 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/30 19:32:26 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CHECKING_HPP
# define ERROR_CHECKING_HPP

# include <map>
# include <string>
# include <vector>

# include "../event_loop/connection.hpp"
# include "../http_request/request_exception.hpp"
# include "../http_request/utility_path.hpp"
# include "../virtual_server/select_location.hpp"
# include "./response_utils.hpp"

# define NO_CONTENT_LENGTH -2
# define INVALID_CONTENT_LENGTH -1
# define OK 0
# define KO 1

bool						client_or_server_error_occured(int const status_code);
int							check_if_all_data_was_read(int& status_code, int const bytes_in_data, int const total_bytes_read);
int							check_if_forbidden(int& status_code, std::pair<std::string, bool> file_location);
int							check_if_file_is_found(int& status_code, std::pair<std::string, bool> file_location);
int							check_request_size(int& status_code, int request_body_size, int content_length_size);
int							check_if_file_is_processable(int& status_code, std::string const& file_location);
int							check_if_file_has_read_permission(int& status_code, std::string const& file_location);

int                         check_if_request_parser_threw_exception(int& status_code, int const client_response_data_status_code);
int							initial_error_checking(int& status_code, Connection& client, RequestData const& request);
int							check_file_status(int& status_code, std::string const& file);

void						check_method(int& status_code, std::string const& method, std::vector<std::string> const& allowed_methods);
void						check_user_information(int& status_code, std::string const& uri_user_information);
void						check_uri_length(int& status_code, std::string const& request_uri);
void						check_http_protocol(int& status_code, std::string const& protocol);
void						check_continue(std::map<std::string, std::string> const& headers);
void						check_request_line_and_headers(Connection const& client, RequestData const& request);

#endif
