/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:50:52 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:11:55 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/connection.hpp"

Connection::Connection(void) {

}

Connection::~Connection(void) {

}

Connection::Connection(Connection const& other) {
	*this = other;
}

Connection&	Connection::operator=(Connection const& other) {
	if (this != &other) {
		this->request_handler = other.request_handler;
		this->response = other.response;
		this->_connection_fd = other._connection_fd;
		this->_request = other._request;
		this->_virtual_servers = other._virtual_servers;
		this->_virtual_server = other._virtual_server;
		this->_time = other._time;
	}
	return (*this);
}

void	Connection::save_body_and_total_bytes_read(void) {
	this->_request.set_body(this->request_handler.get_body());
	this->_request.set_total_bytes_read(this->request_handler.get_total_bytes_read());
}

void	Connection::save_request_line_and_headers(void) {
	this->_request.set_method(this->request_handler.get_request_line_method());
	this->_request.set_uri(this->request_handler.get_request_line_uri());
	this->_request.set_protocol(this->request_handler.get_request_line_protocol());
	this->_request.set_headers(this->request_handler.get_headers());
}

void	Connection::print_request(void) const {
	Color::Modifier blue(Color::FG_BLUE);
	Color::Modifier def(Color::FG_DEFAULT);

	std::cout << blue;
	if (!this->_request.get_method().empty())
		std::cout << this->_request.get_method() << " ";
	if (!this->_request.get_uri().get_path_full().empty())
		std::cout << this->_request.get_uri().get_path_full() << " ";
	if (!this->_request.get_protocol().empty())
		std::cout << this->_request.get_protocol() << "\n";
	if (!this->_request.get_headers().empty()) {
		for (std::map<std::string, std::string>::const_iterator it = this->_request.get_headers().begin(); it != this->_request.get_headers().end(); it++) {
			std::cout << it->first << "=" << it->second << "\n";
		}
	}
	std::cout << "\r\n";
	if (!this->_request.get_body().empty()) {
		if (this->_request.get_body().size() > 1000)
			std::cout << this->_request.get_body().substr(0, 100) << "\n";
		else
			std::cout << this->_request.get_body() << "\n";
	}
	else {
		std::cout << "body is empty\n";
	}
	std::cout << "\n" << def;
}

void	Connection::reset_response_data(void) {
	ResponseData	response_data;

	this->response = response_data;
}

void	Connection::print_response(void) const {
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier def(Color::FG_DEFAULT);

	size_t pos = this->response.get_repsonse_string().find(DOUBLE_CRLF);
	if (pos != std::string::npos) {
		std::string const response_status_line_and_headers = this->response.get_repsonse_string().substr(0, pos + 4);
		std::cout << green << response_status_line_and_headers << def;
	}
}

void	Connection::handle_rewrite(void) {
	for (size_t i = 0; i < 10; i++) {
		std::string const uri_replacement = this->_virtual_server.get_rewrite();
		if (uri_replacement.empty()) {
			return ;
		}
		Uri	new_uri;
		new_uri.parse_uri(uri_replacement);
		this->_request.set_uri(new_uri);
		this->_virtual_server.initialize_virtual_server(this->_request.get_uri().get_authority_host(), this->_request.get_uri().get_path_full(), this->_virtual_servers.second);
	}
}

void	Connection::select_virtual_server(void) {
	this->_virtual_server.initialize_virtual_server(this->_request.get_uri().get_authority_host(), this->_request.get_uri().get_path_full(), this->_virtual_servers.second);
	handle_rewrite();
}

int const&	Connection::get_connection_fd(void) const {
	return (this->_connection_fd);
}

RequestData const&	Connection::get_request(void) const {
	return (this->_request);
}

VirtualServer const&	Connection::get_virtual_server(void) const {
	return (this->_virtual_server);
}

std::pair<int, std::vector<Server> > const&	Connection::get_virtual_servers(void) const {
	return (this->_virtual_servers);
}

time_t const&	Connection::get_time(void) const {
	return (this->_time);
}

void	Connection::set_connection_fd(int const& connection_fd) {
	this->_connection_fd = connection_fd;
}

void	Connection::set_request(RequestData const& request) {
	this->_request = request;
}

void	Connection::set_virtual_server(VirtualServer const& virtual_server) {
	this->_virtual_server = virtual_server;
}

void	Connection::set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers) {
	this->_virtual_servers = virtual_servers;
}

void	Connection::set_time(void) {
	this->_time = time(0);
}
