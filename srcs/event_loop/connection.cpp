/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:50:52 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/02 19:06:44 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/connection.hpp"

Connection::Connection() {

}

Connection::~Connection() {

}

Connection::Connection(Connection const& other) {
	*this = other;
}

Connection&	Connection::operator=(Connection const& other) {
	if (this != &other) {
		this->_virtual_servers = other._virtual_servers;
		this->_request.request_line.method = other._request.request_line.method;
		this->_request.request_line.uri = other._request.request_line.uri;
		this->_request.request_line.protocol = other._request.request_line.protocol;
		this->_request.bytes_in_data = other._request.bytes_in_data;
		this->_request.total_bytes_read = other._request.total_bytes_read;
		this->_request.headers = other._request.headers;
		this->_request.body = other._request.body;
		this->_response = other._response;
	}
	return (*this);
}

std::pair<int, std::vector<Server> > const&	Connection::get_virtual_servers(void) const {
	return (this->_virtual_servers);
}

Connection::t_request const&	Connection::get_request(void) const {
	return (this->_request);
}

Response const&	Connection::get_response(void) const {
	return (this->_response);
}

void	Connection::set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers) {
	this->_virtual_servers = virtual_servers;
}

void	Connection::set_request(Connection::t_request const& request) {
	this->_request.request_line.method = request.request_line.method;
	this->_request.request_line.uri = request.request_line.uri;
	this->_request.request_line.protocol = request.request_line.protocol;
	this->_request.headers = request.headers;
	this->_request.body = request.body;
	this->_request.bytes_in_data = request.bytes_in_data;
	this->_request.total_bytes_read = request.total_bytes_read;
}

void	Connection::print_request(void) const {
	if (!this->_request.request_line.method.empty())
		std::cout << "method = " << this->_request.request_line.method << std::endl;
	std::cout << this->_request.request_line.uri;
	if (!this->_request.request_line.protocol.empty())
		std::cout << "protocol = " << this->_request.request_line.protocol << std::endl;
	if (!this->_request.headers.empty()) {
		std::cout << "headers = " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->_request.headers.begin(); it != this->_request.headers.end(); it++) {
			std::cout << "\t" << it->first << "=" << it->second << std::endl;
		}
	}
	if (!this->_request.body.empty())
		std::cout << this->_request.body << std::endl;
	std::cout << "bytes in client request = " << this->_request.bytes_in_data << std::endl;
	std::cout << "bytes read = " << this->_request.total_bytes_read << std::endl;
}
