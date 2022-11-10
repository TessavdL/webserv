/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/10 12:46:27 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

std::map<int, std::string>	g_status_code_reason_phrase_map = create_status_code_reason_phrase_map();

std::string const&	get_reason_phrase(int status_code) {
	return (g_status_code_reason_phrase_map.find(status_code)->second);
}

void	check_user_information(int& status_code, std::string const& uri_user_information) {
	if (!uri_user_information.empty()) {
		status_code = 400;
	}
}

void	check_if_complete(int& status_code, int const bytes_in_data, int const total_bytes_read) {
	if (bytes_in_data != total_bytes_read) {
		status_code = 400;
	}
}

void	check_method(int& status_code, std::string const& method, std::vector<std::string> const& allowed_methods) {
	if (std::find(allowed_methods.begin(), allowed_methods.end(), method) == allowed_methods.end()) {
		status_code = 405;
	}
}

void	check_request_size(int& status_code, int request_body_size, int content_length_size) {
	if (content_length_size == NO_CONTENT_LENGTH) {
		if (request_body_size != 0) {
			status_code = 400;
		}
	}
	else {
		if (request_body_size > content_length_size) {
			status_code = 413;
		}
	}
}

void	check_uri_length(int& status_code, std::string const& request_uri) {
	if (request_uri.length() > 255) {
		status_code = 414;
	}
}

void	check_http_protocol(int& status_code, std::string const& protocol) {
	if (protocol != "HTTP/1.1") {
		status_code = 505;
	}
}

// void	add_allow_header(std::vector<std::string> allowed_methods, std::string& headers) {
// 	headers.append("Allow");
// 	headers.append(": ");
// 	for (size_t i = 0; i < allowed_methods.size(); i++) {
// 		headers.append(allowed_methods[i]);
// 		if (i + 1 != allowed_methods.size()) {
// 			headers.append(", ");
// 		}
// 	}
// 	headers.append("\r\n");
// }

// void	handle_method(int& status_code, std::string const& method, std::vector<std::string> const& allowed_methods, std::string& headers) {
// 	check_method(status_code, method, allowed_methods);
// 	if (status_code = 405) {
// 		add_allow_header(allowed_methods, headers);
// 	}
// }



/*
incomplete request that keeps connection open
https://stackoverflow.com/questions/69386098/how-to-send-an-incomplete-http-request-using-netcat
*/
