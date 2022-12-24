/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/24 13:54:19 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

bool	client_or_server_error_occured(int const status_code) {
	if (status_code < 399) {
		return (false);
	}
	return (true);
}

// int	check_if_all_data_was_read(int& status_code, int const bytes_in_data, int const total_bytes_read) {
// 	if (bytes_in_data != total_bytes_read) {
// 		status_code = 400;
// 		return (KO);
// 	}
// 	return (OK);
// }

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

int	check_if_file_is_processable(int& status_code, std::string const& file_location) {
	if (!is_regular_file_stat(file_location.c_str())) {
		status_code = 403;
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
	std::map<std::string, std::string>::const_iterator it = headers.find("Content-Length");

	if (it != headers.end()) {
		char	*p;
		long	number = strtol(it->second.c_str(), &p, 10);
		if (*p) {
			return (INVALID_CONTENT_LENGTH);
		}
		else {
			return (number);
		}	
	}
	return (NO_CONTENT_LENGTH);
}

int	check_if_request_parser_threw_exception(int& status_code, int const client_response_data_status_code) {
	if (client_response_data_status_code > 399) {
		status_code = client_response_data_status_code;
		return (KO);
	}
	else if (client_response_data_status_code == 100) {
		status_code = 100;
		return (KO);
	}
	return (OK);
}

static int	is_valid_cgi(int& status_code, Connection& client, RequestData const& request) {
	if (is_cgi(request.get_uri().get_path_full() && client.get_virtual_server().get_cgi().first.empty())) {
		status_code = 400;
		return (KO);
	}
	return (OK);
}

int	initial_error_checking(int& status_code, Connection& client, RequestData const& request) {
	long	content_length = find_content_length(request.get_headers());

	if (check_if_request_parser_threw_exception(status_code, client.get_response().get_status_code())) {
		return (status_code);
	}
	// if (check_if_all_data_was_read(status_code, request.get_bytes_in_data(), request.get_total_bytes_read())) {
	// 	std::cout << "HELLO2" << std::endl;
	// 	return (status_code);
	// }
	if (content_length == INVALID_CONTENT_LENGTH) {
		std::cout << "HELLO3" << std::endl;
		status_code = 400;
		return (status_code);
	}
	if (check_request_size(status_code, request.get_body().length(), content_length)) {
		std::cout << "HELLO4" << std::endl;
		return (status_code);
	}
	if (is_valid_cgi(status_code, client, request)) {
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

// ERRORS IN REQUEST STATUS LINE

void	check_method(std::string const& method, std::vector<std::string> const& allowed_methods) {
	if (std::find(allowed_methods.begin(), allowed_methods.end(), method) == allowed_methods.end()) {
		throw (RequestException(405, "check_method"));
	}
}

void	check_uri_length(std::string const& request_uri) {
	if (request_uri.length() > 2048) {
		throw (RequestException(414, "check_uri_length"));
	}
}

void	check_user_information(std::string const& uri_user_information) {
	if (!uri_user_information.empty()) {
		throw (RequestException(400, "check_user_information"));
	}
}

void	check_http_protocol(std::string const& protocol) {
	if (protocol != "HTTP/1.1") {
		throw (RequestException(400, "check_http_protocol"));
	}
}

void	error_check_request_line_and_headers(Connection const& client, RequestData const& request) {
	check_method(request.get_method(), client.get_virtual_server().get_limit_except());
	check_uri_length(request.get_uri().get_path_full());
	check_user_information(request.get_uri().get_authority_user_information());
	check_http_protocol(request.get_protocol());
}
