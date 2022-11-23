/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 12:55:38 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

bool	client_or_server_error_occured(int const status_code) {
	if (status_code == 200) {
		return (false);
	}
	return (true);
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

int	check_if_forbidden(int& status_code, std::pair<std::string, bool> file_location) {
	if (!file_location.first.empty() && file_location.second == false) {
		status_code = 403;
		return (KO);
	}
	return (OK);
}

int	check_if_file_is_found(int& status_code, std::pair<std::string, bool> file_location) {
	if (file_location.first.empty() && file_location.second == false) {
		status_code = 404;
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

static long	find_content_length(std::map<std::string, std::string> headers) {
	std::map<std::string, std::string>::const_iterator it = headers.find("Content-length");

	if (it != headers.end()) {
		char	*p;
		long	number = strtol(it->second.c_str(), &p, 10);
		if (*p) {
			return (number);
		}
		else {
			return (INVALID_CONTENT_LENGTH);
		}	
	}
	return (NO_CONTENT_LENGTH);
}

int	initial_error_checking(int& status_code, Connection& client, Connection::t_request const& request) {
	long	content_length = find_content_length(request.headers);

	if (check_user_information(status_code, request.request_line.uri.get_authority_user_information())) {
		return (status_code);
	}
	if (check_if_complete(status_code, request.bytes_in_data, request.total_bytes_read)) {
		return (status_code);
	}
	if (check_method(status_code, request.request_line.method, client.get_virtual_server().get_limit_except())) {
		return (status_code);
	}
	if (content_length == INVALID_CONTENT_LENGTH) {
		status_code = 400;
		return (status_code);
	}
	if (check_request_size(status_code, request.body.size(), content_length)) {
		return (status_code);
	}
	if (check_http_protocol(status_code, request.request_line.protocol)) {
		return (status_code);
	}
	return (OK);
}

int	check_file_status(int& status_code, std::string const& file) {
	if (check_if_file_is_processable(status_code, file)) {
		return (status_code);
	}
	if (check_if_file_has_read_permission(status_code, file.c_str())) {
		return (status_code);
	}
	return (OK);
}

/*
incomplete request that keeps connection open
https://stackoverflow.com/questions/69386098/how-to-send-an-incomplete-http-request-using-netcat
*/
