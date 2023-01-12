/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_checking.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 20:07:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/12 16:19:43 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

bool	client_or_server_error_occured(int const status_code) {
	if (status_code < 399) {
		return (false);
	}
	return (true);
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

int	check_request_size(int& status_code, int request_body_size, int content_length_size) {
	if (content_length_size == NO_CONTENT_LENGTH) {
		if (request_body_size != 0) {
			status_code = 400;
			return (KO);
		}
	}
	else {
		if (request_body_size != content_length_size) {
			cout << request_body_size << " <<< REQUEST BODY SIZE\t" << content_length_size << " <<< CONTENT LENGTH SIZE" << endl;
			status_code = 400;
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
	if (is_cgi(request.get_uri().get_path_full()) && client.get_virtual_server().get_cgi().first.empty()) {
		status_code = 400;
		return (KO);
	}
	return (OK);
}

static bool	is_not_chunked(RequestData const& request_data) {
	std::map<std::string, std::string>					headers = request_data.get_headers();
	std::map<std::string, std::string>::const_iterator	it = headers.find("Transfer-Encoding");
	
	if (it != headers.end()) {
		if (!it->second.compare("chunked")) {
			return (false);
		}
		return (true);
	}
	return (true);
}

static bool	is_empty_request(RequestData const& request) {
	if (request.get_method().empty() || request.get_protocol().empty() || request.get_headers().empty()) {
		return (true);
	}
	return (false);
}

int	initial_error_checking(int& status_code, Connection& client, RequestData const& request) {
	long	content_length = find_content_length(request.get_headers());

	if (check_if_request_parser_threw_exception(status_code, client.response.get_status_code())) {
		return (status_code);
	}
	if (is_empty_request(request)) {
		status_code = 400;
	}
	if (content_length == INVALID_CONTENT_LENGTH) {
		status_code = 400;
		return (status_code);
	}
	if (is_not_chunked(request) && check_request_size(status_code, request.get_body().size(), content_length)) {
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

void	check_client_max_body_size(int client_max_body_size, std::string content_length) {
	if (client_max_body_size != 0 && (atoi(content_length.c_str()) > (client_max_body_size * 1000000))) {
		throw (RequestException(413, "check_client_max_body_size"));
	}
}

void	check_continue(std::map<std::string, std::string> const& headers) {
	std::map<std::string, std::string>::const_iterator	expect_header = headers.find("Expect");
	
	if (expect_header != headers.end()) {
		std::string const	expectation = string_to_lower(expect_header->second);
		if (!expectation.compare("100-continue")) {
			throw (RequestException(100, "check_continue"));
		}
		else {
			throw (RequestException(417, "check_continue"));
		}
	}
}

void	check_request_line_and_headers(Connection const& client, RequestData const& request) {
	check_method(request.get_method(), client.get_virtual_server().get_limit_except());
	check_uri_length(request.get_uri().get_path_full());
	check_user_information(request.get_uri().get_authority_user_information());
	check_http_protocol(request.get_protocol());
	check_client_max_body_size(client.get_virtual_server().get_client_max_body_size(), request.get_headers().find("Content-Length")->second);
	check_continue(request.get_headers());
}
