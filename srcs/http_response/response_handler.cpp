/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:44:59 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/21 15:48:09 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_handler.hpp"


ResponseHandler::ResponseHandler() : _status_code(200) {

}

ResponseHandler::~ResponseHandler(void) {

}

ResponseHandler::ResponseHandler(ResponseHandler const& other) {
	*this = other;
}

ResponseHandler&	ResponseHandler::operator=(ResponseHandler const& other) {
	if (this != &other) {
		this->_status_code = other._status_code;
	}
	return (*this);
}

bool	ResponseHandler::client_or_server_error_occured(void) const {
	if (this->_status_code == 200)
		return (false);
	return (true);
}

static std::string	get_file_contents(std::string const& file_location) {
	std::ifstream	input_stream;
	std::string		file_contents;
	
	std::cout << file_location << std::endl;
	input_stream.open(file_location);
	if (!input_stream) {
		throw (FatalException("OPEN in get_file_contents\n"));
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		file_contents.append(ss.str());
	}
	input_stream.close();
	return (file_contents);
}

void	ResponseHandler::create_error_response(Connection& client, std::string const& file_location) {
	ResponseData	response_data;

	std::string body = get_file_contents(file_location);
	body.replace(body.find("STATUS_CODE"), strlen("STATUS_CODE"), std::to_string(this->_status_code));
	body.replace(body.find("REASON_PHRASE"), strlen("REASON_PHRASE"), get_reason_phrase(this->_status_code));
	body.replace(body.find("STATUS_CODE"), strlen("STATUS_CODE"), std::to_string(this->_status_code));
	body.replace(body.find("REASON_PHRASE"), strlen("REASON_PHRASE"), get_reason_phrase(this->_status_code));
	if (!body.empty())
		response_data.set_body(body);
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(client, file_location, body.length()));
	client.set_response(response_data);
}

void	ResponseHandler::handle_response(Connection& client) {
	Connection::t_request request = client.get_request();

	initial_error_checking(this->_status_code, client, request);
	if (client_or_server_error_occured()) {
		std::string file = find_error_page_location(this->_status_code, client.get_virtual_server());
		return (create_error_response(client, file));
	}
	if (!request.request_line.method.compare("GET")) {
		handle_get_request(client, request);
	}
	// if (request.request_line.method.compare("POST")) {

	// }
	// if (request.request_line.method.compare("DELETE")) {

	// }
}

// bool	isCGI(std::string const& file_location) {
// 	size_t pos = file_location.find(".");

// 	if (pos != std::string::npos) {
// 		std::pair<string, string> p = split_string_in_half(file_location, ".")
// 		if (p.second )
// 	}
// }

static std::string create_content_type(std::string const& file_name) {
	std::string const& extension = file_name.substr(file_name.find("."));

	if (extension.empty()) {
		return ("application/octet-stream");
	}
	else if (!extension.compare(".html")) {
		return ("text/html");
	}
	else if (!extension.compare(".css")) {
		return ("text/css");
	}
	else if (!extension.compare(".js")) {
		return ("text/javascript");
	}
	else if (!extension.compare(".jpg")) {
		return ("image/jpeg");
	}
	return ("text/plain");
}

static std::string	create_allowed_methods(std::vector<std::string> allowed_methods) {
	std::string	ret;
	
	for (std::vector<std::string>::const_iterator it = allowed_methods.begin(); it != allowed_methods.cend(); it++) {
		ret.append(*it);
		if (!is_last_iterator(it, allowed_methods.cend())) {
			ret.append(", ");
		}
	}
	return (ret);
}

std::map<std::string, std::string>	ResponseHandler::create_headers(Connection& client, std::string const& file_name, size_t body_size) {
	std::map<std::string, std::string>	headers;

	headers["Content-Length"] = std::to_string(body_size);
	headers["Content-Type"] = create_content_type(file_name);
	if (this->_status_code == 405) {
		headers["Allow"] = create_allowed_methods(client.get_virtual_server().get_limit_except());
	}
	// for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++) {
	// 	std::cout << it->first << " " << it->second << std::endl;
	// }
	return (headers);
}

void	ResponseHandler::create_get_response(Connection& client, std::string const& file_location) {
	ResponseData	response_data;

	std::string body = get_file_contents(file_location);
	if (!body.empty()) {
		response_data.set_body(body);
	}
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(client, file_location, body.size()));
	client.set_response(response_data);
}

void	ResponseHandler::handle_get_request(Connection& client, Connection::t_request const& request) {
	std::pair<std::string, bool>	file_location = handle_file_location(client.get_virtual_server().get_index(), request.request_line.uri.get_path_full());
	std::string						file = file_location.first;

	if (check_if_file_is_found(this->_status_code, file_location.second)) {
		file = find_error_page_location(this->_status_code, client.get_virtual_server());
		return (create_error_response(client, file));
	}
	if (check_file_status(this->_status_code, file)) {
		file = find_error_page_location(this->_status_code, client.get_virtual_server());
		return (create_error_response(client, file));
	}
	// std::cout << file_location << std::endl;
	// if (isCGI(file_location)) {
	// 	create_cgi_response(client, file_location, "GET");
	// }
	create_get_response(client, file);
}

