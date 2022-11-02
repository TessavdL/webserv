/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/01 20:55:24 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/error_checking.hpp"

std::map<int, std::string>	g_status_code_reason_phrase_map = create_status_code_reason_phrase_map();

std::string const&	get_reason_phrase(int status_code) {
	return (g_status_code_reason_phrase_map.find(status_code)->second);
}

void	check_uri_length(int& status_code, std::string& reason_phrase, std::string const& request_uri) {
	if (request_uri.length() > 255) {
		status_code = 414;
		reason_phrase = get_reason_phrase(status_code);
	}
}

void	check_if_complete(int& status_code, std::string& reason_phrase, int bytes_in_data, int total_bytes_read) {
	if (bytes_in_data != total_bytes_read) {
		status_code = 400;
		reason_phrase = get_reason_phrase(status_code);
	}
}

void	check_request_size(int& status_code, std::string& reason_phrase, int request_body_size, int content_length_size) {
	if (request_body_size > content_length_size) {
		status_code = 413;
		reason_phrase = get_reason_phrase(status_code);
	}
}

void	check_method(int& status_code, std::string& reason_phrase, std::string const& method, std::vector<std::string> const& allowed_methods) {
	if (std::find(allowed_methods.begin(), allowed_methods.end(), method) == allowed_methods.end()) {
		status_code = 405;
		reason_phrase = get_reason_phrase(status_code);
	}
}

void	add_allow_header(std::vector<std::string> allowed_methods, std::string& headers) {
	headers.append("Allow");
	headers.append(": ");
	for (size_t i = 0; i < allowed_methods.size(); i++) {
		headers.append(allowed_methods[i]);
		if (i + 1 != allowed_methods.size()) {
			headers.append(", ");
		}
	}
	headers.append("\r\n");
}

void	handle_method(int& status_code, std::string& reason_phrase, std::string const& method, std::vector<std::string> const& allowed_methods, std::string& headers) {
	check_method(status_code, reason_phrase, method, allowed_methods);
	if (status_code = 405) {
		add_allow_header(allowed_methods, headers);
	}
}

/*
incomplete request that keeps connection open
https://stackoverflow.com/questions/69386098/how-to-send-an-incomplete-http-request-using-netcat
*/

void	check_http_protocol(int& status_code, std::string& reason_phrase, std::string const& protocol) {
	if (protocol != "HTTP1/1") {
		status_code = 505;
		reason_phrase = get_reason_phrase(status_code);
	}
}

