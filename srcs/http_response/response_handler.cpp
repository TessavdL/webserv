/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:44:59 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/12 13:21:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_handler.hpp"

ResponseHandler::ResponseHandler() : _status_code(200), _state(UNSET) {

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

// HANDLE RESPONSE

void	ResponseHandler::handle_response(Connection& client) {
	RequestData request = client.get_request();

	// initial_error_checking(this->_status_code, client, request);
	if (client_or_server_error_occured(this->_status_code)) {
		std::string const error_page = handle_error_page(client.get_virtual_server());
		if (this->_state == DEFAULT_ERROR) {
			return (create_error_response(client, default_error_page_location(), default_error_page_content()));
		}
		else if (this->_state == CUSTOM_ERROR) {
			return (create_error_response(client, error_page, get_file_content(error_page)));
		}
	}
	if (!request.get_method().compare("GET")) {
		handle_get_response(client, request);
	}
	if (!request.get_method().compare("POST")) {
		handle_post_response(client, request);
	}
	// if (request.request_line.method.compare("DELETE")) {

	// }
}

void		ResponseHandler::handle_post_response(Connection& client, RequestData const& request) {
	std::string						file_path = create_path(client.get_virtual_server().get_root(), request.get_uri().get_path_full());
	std::pair<std::string, bool>	file_location = search_for_file_to_serve(client.get_virtual_server().get_index(), file_path);
	std::string						file = file_location_handler(client.get_virtual_server(), file_location);

	if (client_or_server_error_occured(this->_status_code)) {
		if (this->_state == DEFAULT_ERROR) {
			return (create_error_response(client, default_error_page_location(), default_error_page_content()));
		}
		else if (this->_state == CUSTOM_ERROR) {
			return (create_error_response(client, file, get_file_content(file)));
		}
	}
	if (isCGI(file)) {
		create_cgi_response(client, file);
		this->_state = CGI;
		return ;
	}
}

std::string	ResponseHandler::error_page_location_handler(std::pair<std::string, bool> error_page) {
	if (check_if_file_is_found(this->_status_code, error_page) || check_if_forbidden(this->_status_code, error_page) || check_file_status(this->_status_code, error_page.first)) {
		this->_state = DEFAULT_ERROR;
		return ("");
	}
	this->_state = CUSTOM_ERROR;
	return (error_page.first);
}

std::string	ResponseHandler::handle_error_page(VirtualServer const& virtual_server) {
	std::string const	error_page = find_error_page_location(this->_status_code, virtual_server);

	if (error_page.empty()) {
		this->_state = DEFAULT_ERROR;
		return ("");
	}
	else {
		return (error_page_location_handler(search_for_file_to_serve(virtual_server.get_index(), error_page)));
	}
}

std::string	ResponseHandler::file_location_handler(VirtualServer const& virtual_server, std::pair<std::string, bool> const& file_location) {
	std::string const	file = file_location.first;

	if (check_if_file_is_found(this->_status_code, file_location)) {
		return (handle_error_page(virtual_server));
	}
	if (check_if_forbidden(this->_status_code, file_location)) {
		if (check_if_auto_index_is_on(virtual_server.get_autoindex())) {
			this->_status_code = 200;
			this->_state = DIRECTORY_LIST;
			return (create_directory_list_page(file));
		}
		else {
			return (handle_error_page(virtual_server));
		}
	}
	if (check_file_status(this->_status_code, file)) {
		return (handle_error_page(virtual_server));
	}
	return (file);
}

void	ResponseHandler::handle_get_response(Connection& client, RequestData const& request) {
	std::string						file_path = create_path(client.get_virtual_server().get_root(), request.get_uri().get_path_full());
	std::pair<std::string, bool>	file_location = search_for_file_to_serve(client.get_virtual_server().get_index(), file_path);
	std::string						file = file_location_handler(client.get_virtual_server(), file_location);

	// std::cout << "file path = " << file_path << std::endl;
	// std::cout << "file location = " << file_location.first << std::endl;
	// std::cout << "file = " << file << std::endl;
	// std::cout << "status_code = " << this->_status_code << std::endl;
	if (client_or_server_error_occured(this->_status_code)) {
		if (this->_state == DEFAULT_ERROR) {
			return (create_error_response(client, default_error_page_location(), default_error_page_content()));
		}
		else if (this->_state == CUSTOM_ERROR) {
			return (create_error_response(client, file, get_file_content(file)));
		}
	}
	if (isCGI(file)) {
		create_cgi_response(client, file);
		this->_state = CGI;
		return ;
	}
	if (this->_state == DIRECTORY_LIST) {
		return (create_directory_list_response(client, file));
	}
	this->_state = GET;
	create_get_response(client, file, get_file_content(file));
}

// CREATE RESPONSE

void	ResponseHandler::create_error_response(Connection& client, std::string const& file_location, std::string const& file_content) {
	ResponseData	response_data;
	std::string		body = file_content;

	if (!body.empty()) {
		body.replace(body.find("$STATUS_CODE"), strlen("$STATUS_CODE"), std::to_string(this->_status_code));
		body.replace(body.find("$REASON_PHRASE"), strlen("$REASON_PHRASE"), get_reason_phrase(this->_status_code));
		body.replace(body.find("$STATUS_CODE"), strlen("$STATUS_CODE"), std::to_string(this->_status_code));
		body.replace(body.find("$REASON_PHRASE"), strlen("$REASON_PHRASE"), get_reason_phrase(this->_status_code));
		response_data.set_body(body);
	}
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(client, file_location, body.length()));
	client.set_response(response_data);
}

void	ResponseHandler::create_directory_list_response(Connection& client, std::string const& page) {
	ResponseData	response_data;

	response_data.set_body(page);
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(client, ".html", page.length()));
	client.set_response(response_data);
}

void	ResponseHandler::create_get_response(Connection& client, std::string const& file_location, std::string file_content) {
	ResponseData	response_data;
	std::string		body = file_content;

	if (!body.empty()) {
		response_data.set_body(body);
	}
	response_data.set_status_code(this->_status_code);
	response_data.set_reason_phrase(get_reason_phrase(this->_status_code));
	response_data.set_headers(create_headers(client, file_location, body.length()));
	client.set_response(response_data);
}

// CREATE RESPONSE HELPERS

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

	if (this->_status_code == 405) {
		headers["Allow"] = create_allowed_methods(client.get_virtual_server().get_limit_except());
	}
	headers["Connection"] = "close";
	headers["Content-Length"] = std::to_string(body_size);
	headers["Content-Type"] = create_content_type(file_name);
	headers["Server"] = "Codyserv (macOS)";
	return (headers);
}

// only use when file exist and read permissions are in order
std::string	ResponseHandler::get_file_content(std::string const& file_location) {
	std::ifstream	input_stream;
	std::string		file_contents;

	input_stream.open(file_location);
	if (!input_stream || input_stream.bad()) {
		throw (FatalException("SYSCALL open in get_file_contents\n"));
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		file_contents.append(ss.str());
	}
	input_stream.close();
	return (file_contents);
}

ResponseHandler::state const&	ResponseHandler::get_status(void) const {
	return (this->_state);
}
