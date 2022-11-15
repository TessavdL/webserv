/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:44:59 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 14:41:31 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_handler.hpp"

ResponseHandler::ResponseHandler(Connection& client) {

}

ResponseHandler::~ResponseHandler(void) {

}

ResponseHandler::ResponseHandler(ResponseHandler const& other) {
	*this = other;
}

ResponseHandler&	ResponseHandler::operator=(ResponseHandler const& other) {
	// if (this != &other) {

	// }
	return (*this);
}

bool	ResponseHandler::client_or_server_error_occured(void) const {
	if (this->_status_code == 200)
		return (false);
	return (true);
}

void	ResponseHandler::create_error_response(Connection& client) {
	ResponseData	response_data;

	// client.set_response_data(response_data);
}

void	ResponseHandler::handle_response(Connection& client) {
	Connection::t_request request = client.get_request();

	this->_status_code = initial_error_checking(client, request);
	if (client_or_server_error_occured) {
		return (create_error_response(client));
	}
	if (request.request_line.method.compare("GET")) {
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

static std::string	get_file_contents(std::string const& file_location) const {
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

void	ResponseHandler::create_get_response(Conncetion& client, std::string const& file_location) {
	ResponseData	response_data;

	response_data.set_status_code(this->_status_code);
	response_data.set_body(get_file_contents(file_location));
	respones_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	// headers?
}

static std::string	find_file(Connection& client, std::string const& path) {
	// get cwd append root_plus_uri_path
	
}

void	ResponesHandler::handle_get_request(Connection& client, Connection::t_request request) {
	std::string	file_location;
	
	file_location = find_file(client, request.request_line.uri.get_full_path());
	if (file_location.empty) {
		this->_status_code = 404;
		return (create_error_response(client));
	}
	// if (isCGI(file_location)) {
	// 	create_cgi_response(client, file_location, "GET");
	// }
	create_get_response(client, file_location);
}
