/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:50:52 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/01 17:40:02 by tevan-de      ########   odam.nl         */
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
		// this->_connection_fd = other._connection_fd;
		this->_virtual_servers = other._virtual_servers;
		this->_request = other._request;
		this->_response = other._response;
	}
	return (*this);
}

// int const&	Connection::get_connection_fd(void) const {
// 	return (this->_connection_fd);
// }

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
