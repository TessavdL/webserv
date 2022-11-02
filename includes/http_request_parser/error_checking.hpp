/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:15 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/02 19:11:14 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_CHECKING_HPP
# define ERROR_CHECKING_HPP

#include <map>
#include <string>
#include <vector>

std::string const&			get_reason_phrase(int status_code);
void	                    check_user_information(int& status_code, std::string& reason_phrase, std::string const& uri_user_information);
void	                    check_uri_length(int& status_code, std::string& reason_phrase, std::string const& request_uri);
void	                    check_if_complete(int& status_code, std::string& reason_phrase, int bytes_in_data, int total_bytes_read);
void	                    check_request_size(int& status_code, std::string& reason_phrase, int request_body_size, int content_length_size);
void	                    check_method(int& status_code, std::string& reason_phrase, std::string const& method, std::vector<std::string> const& allowed_methods);
void	                    add_allow_header(std::vector<std::string> allowed_methods, std::string& headers);
void	                    handle_method(int& status_code, std::string& reason_phrase, std::string const& method, std::vector<std::string> const& allowed_methods, std::string& headers);
void	                    check_http_protocol(int& status_code, std::string& reason_phrase, std::string const& protocol);

#endif
