/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   initial_error_checking.cpp                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 15:06:31 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/09 16:38:37 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/error_checking.hpp"

#define NO_LOCATION -2
#define NO_CONTENT_LENGTH -2
#define INVALID_CONTENT_LENGTH -1

static std::vector<std::string>	standard_method(void) {
	std::vector<std::string>	standard_method;

	standard_method.push_back("GET");
	standard_method.push_back("POST");
	standard_method.push_back("DELETE");
	return (standard_method);
}

static std::vector<std::string>	find_allowed_methods(Connection const& client) {
	int const&	location_index = client.get_location_index();
	Server const&	virtual_server = client.get_virtual_servers().second[client.get_server_index()];

	if (location_index != NO_LOCATION) {
		if (!virtual_server.get_location_block()[location_index].get_limit_except().empty()) {
			return (virtual_server.get_location_block()[location_index].get_limit_except());
		}
		else {
			return (standard_method());
		}
	}
	else {
		return (standard_method());
	}
}

static long	find_content_length(std::map<std::string, std::string> headers) {
	std::map<std::string, std::string>::const_iterator it = headers.find("Content-length");

	if (it != headers.end()) {
		char *p;
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

static std::pair<int, std::string>	return_status_code_and_reason_phrase(int status_code) {
	return (std::pair<int, std::string>(status_code, get_reason_phrase(status_code)));
}

std::pair<int, std::string>	initial_error_checking(Connection& client, Connection::t_request const& request) {
	int	status_code = 200;

	check_user_information(status_code, request.request_line.uri.get_authority_user_information());
	if (status_code != 200) {
		return (return_status_code_and_reason_phrase(status_code));
	}
	check_if_complete(status_code, request.bytes_in_data, request.total_bytes_read);
	if (status_code != 200) {
		return (return_status_code_and_reason_phrase(status_code));
	}
	check_method(status_code, request.request_line.method, find_allowed_methods(client));
	if (status_code != 200) {
		return (return_status_code_and_reason_phrase(status_code));
	}
	if (!request.body.empty()) {
		long	content_length = find_content_length(request.headers);
		if (content_length == INVALID_CONTENT_LENGTH/* || content_length == NO_CONTENT_LENGTH*/) {
			status_code = 400;
			return (return_status_code_and_reason_phrase(status_code));
		}
		check_request_size(status_code, request.body.size(), content_length);
		if (status_code != 200) {
			return (return_status_code_and_reason_phrase(status_code));
		}
	}
	// check_uri_length(status_code, request.request_line.uri.
	// need to save total uri length someone or handle it while parsing and just throw the 414
	check_http_protocol(status_code, request.request_line.protocol);
	return (return_status_code_and_reason_phrase(status_code));
}
