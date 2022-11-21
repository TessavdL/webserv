/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/21 13:40:18 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

std::map<int, std::string>	g_status_code_reason_phrase_map = create_status_code_reason_phrase_map();

std::string const&	get_reason_phrase(int status_code) {
	return (g_status_code_reason_phrase_map.find(status_code)->second);
}

int	check_user_information(int& status_code, std::string const& uri_user_information) {
	if (!uri_user_information.empty()) {
		status_code = 400;
		return (KO);
	}
	return (OK);
}

int	check_if_complete(int& status_code, int const bytes_in_data, int const total_bytes_read) {
	if (bytes_in_data != total_bytes_read) {
		status_code = 400;
		return (KO);
	}
	return (OK);
}

int	check_method(int& status_code, std::string const& method, std::vector<std::string> const& allowed_methods) {
	if (std::find(allowed_methods.begin(), allowed_methods.end(), method) == allowed_methods.end()) {
		status_code = 405;
		return (KO);
	}
	return (OK);
}

int	check_request_size(int& status_code, int request_body_size, int content_length_size) {
	if (content_length_size == NO_CONTENT_LENGTH) {
		if (request_body_size != 0) {
			status_code = 400;
			return (KO);
		}
	}
	else {
		if (request_body_size != content_length_size) {
			status_code = 413;
			return (KO);
		}
	}
	return (OK);
}

int	check_uri_length(int& status_code, std::string const& request_uri) {
	if (request_uri.length() > 255) {
		status_code = 414;
		return (KO);
	}
	return (OK);
}

int	check_http_protocol(int& status_code, std::string const& protocol) {
	if (protocol != "HTTP/1.1") {
		status_code = 505;
		return (KO);
	}
	return (OK);
}

int	check_if_file_is_processable(int& status_code, std::string const& file_location) {
	if (!is_regular_file_stat(file_location.c_str())) {
		status_code = 422;
		return (KO);
	}
	return (OK);
}

int	check_if_file_has_read_permission(int& status_code, std::string const& file_location) {
	if (!has_read_permission(file_location.c_str())) {
		status_code = 403;
		return (KO);
	}
	return (OK);
}

bool	check_if_file_is_found(int& status_code, bool file_location) {
	if (file_location == false) {
		status_code = 404;
		return (KO);
	}
	return (OK);
}


/*
incomplete request that keeps connection open
https://stackoverflow.com/questions/69386098/how-to-send-an-incomplete-http-request-using-netcat
*/
