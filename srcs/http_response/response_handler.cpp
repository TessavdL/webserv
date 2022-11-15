/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:44:59 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 18:57:39 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_handler.hpp"
#include <unistd.h>//for getcwd

// ResponseHandler::ResponseHandler(Connection& client) {

// }

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

void	ResponseHandler::create_error_response(Connection& client) {
	ResponseData	response_data;

	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(0));
	client.set_response(response_data);
}

void	ResponseHandler::handle_response(Connection& client) {
	Connection::t_request request = client.get_request();

	this->_status_code = initial_error_checking(client, request);
	if (client_or_server_error_occured()) {
		return (create_error_response(client));
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

static std::string	get_file_contents(std::string const& file_location) {
	std::ifstream	input_stream;
	std::string		file_contents;
	
	input_stream.open(file_location);
	if (!input_stream) {
		throw (FatalException());
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		file_contents.append(ss.str());
	}
	input_stream.close();
	return (file_contents);
}

std::map<std::string, std::string>	ResponseHandler::create_headers(size_t body_size) {
	std::map<std::string, std::string>	headers;

	headers["Content-Length"] = std::to_string(body_size);
	return (headers);
}

void	ResponseHandler::create_get_response(Connection& client, std::string const& file_location) {
	ResponseData	response_data;

	std::string body = get_file_contents(file_location);
	if (!body.empty())
		response_data.set_body(body);
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(body.length()));
	// std::cout << "size of headers in create get repsonse " << response_data.get_headers().size() << std::endl;
	client.set_response(response_data);
}

static std::string	find_file(std::string const& root, std::string const& uri_path) {
	char		*buf;
	std::string	current_working_directory;
	
	buf = getcwd(NULL, 0);
	// if get cwd fails throw fatal
	current_working_directory = std::string(buf);
	free(buf);
	return (current_working_directory + "/" + root_plus_uri_path(root, uri_path));
}

void	ResponseHandler::handle_get_request(Connection& client, Connection::t_request request) {
	std::string	file_location;
	
	file_location = find_file(client.get_virtual_server().get_root(), request.request_line.uri.get_path_full());
	std::cout << file_location << std::endl;

	if (file_location.empty()) {
		this->_status_code = 404;
		return (create_error_response(client));
	}
	// if (isCGI(file_location)) {
	// 	create_cgi_response(client, file_location, "GET");
	// }
	create_get_response(client, file_location);
}
