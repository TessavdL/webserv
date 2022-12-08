/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:50:52 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/08 15:05:59 by tevan-de      ########   odam.nl         */
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
		this->_request = other._request;
		this->_response = other._response;
		this->_connection_fd = other._connection_fd;
	}
	return (*this);
}

void	Connection::print_request(void) const {
	if (!this->_request.get_method().empty())
		std::cout << "method = " << this->_request.get_method() << std::endl;
	std::cout << this->_request.get_uri();
	if (!this->_request.get_protocol().empty())
		std::cout << "protocol = " << this->_request.get_protocol() << std::endl;
	if (!this->_request.get_headers().empty()) {
		std::cout << "headers = " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->_request.get_headers().begin(); it != this->_request.get_headers().end(); it++) {
			std::cout << "\t" << it->first << "=" << it->second << std::endl;
		}
	}
	if (!this->_request.get_body().empty())
		std::cout << this->_request.get_body() << std::endl;
	std::cout << "bytes in client request = " << this->_request.get_bytes_in_data() << std::endl;
	std::cout << "bytes read = " << this->_request.get_total_bytes_read() << std::endl;
}

void	Connection::select_virtual_server(void) {
	this->_virtual_server.initialize_virtual_server(this->_request.get_uri().get_authority_host(), this->_request.get_uri().get_path_full(), this->_virtual_servers.second);
	// this->_server_index = determine_server_index(this->_request.request_line.uri.get_authority_host(), this->get_virtual_servers().second);
	// this->_location_index = determine_location_index(this->_request.request_line.uri.get_path_full(), this->_virtual_servers.second[this->_server_index].get_location_block());
}

int const&	Connection::get_connection_fd(void) const {
	return (this->_connection_fd);
}

ResponseData const&	Connection::get_response(void) const {
	return (this->_response);
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

// size_t const&	Connection::get_location_index(void) const {
// 	return (this->_location_index);
// }

// size_t const&	Connection::get_server_index(void) const {
// 	return (this->_server_index);
// }

void	Connection::set_connection_fd(int const& connection_fd) {
	this->_connection_fd = connection_fd;
}

void	Connection::set_response(ResponseData const& response) {
	this->_response = response;
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

// void	Connection::set_location_index(size_t location_index) {
// 	this->_location_index = location_index;
// }

// void	Connection::set_server_index(size_t server_index) {
// 	this->_server_index = server_index;
// }
