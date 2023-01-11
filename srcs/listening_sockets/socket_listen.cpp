/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_listen.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 15:03:35 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/11 15:56:45 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/listening_sockets/socket_listen.hpp"

#include <iostream>

SocketListen::SocketListen(int port_number) : _port_number(port_number) {
	this->create_socket();
	this->define_socket_address();
	this->bind_socket_to_address();
	this->listen_on_socket();
}

SocketListen::~SocketListen(void) {

}

SocketListen::SocketListen(SocketListen const& other) {
	*this = other;
}

SocketListen&		SocketListen::operator=(SocketListen const& other) {
	if (this != &other) {
		this->_addr.sin_family = other._addr.sin_family;
		this->_addr.sin_addr.s_addr = other._addr.sin_addr.s_addr;
		this->_addr.sin_port = other._addr.sin_port;
		this->_fd = other._fd;
		this->_port_number = other._port_number;
	}
	return (*this);
}

void				SocketListen::create_socket(void) {
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1) {
		throw (FatalException("System call to socket in SocketListen::create_socket()\n"));
	}
	int	status = fcntl(this->_fd, F_SETFL, O_NONBLOCK);
	if (status == -1) {
		throw (FatalException("System call to fcntl in SocketListen::create_socket()\n"));
	}
	int	n = 1;
	status = setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));
	if (status == -1) {
		throw (FatalException("System call to setsockopt in SocketListen::create_socket()\n"));
	}
	status = setsockopt(this->_fd, SOL_SOCKET, SO_NOSIGPIPE, &n, sizeof(n));
	if (status == -1) {
		throw (FatalException("System call to setsockopt in SocketListen::create_socket()\n"));
	}
}

void	SocketListen::define_socket_address(void) {
	bzero((char *)&this->_addr, sizeof(this->_addr));
	this->_addr.sin_family = AF_INET;
	this->_addr.sin_addr.s_addr = INADDR_ANY;
	this->_addr.sin_port = htons(this->_port_number);
}

void	SocketListen::bind_socket_to_address(void) {
	int	status = bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof(this->_addr));
	if (status == -1) {
		throw (FatalException("System call to bind in SocketListen::bind_socket_to_address()\n"));
	}
}

void	SocketListen::listen_on_socket(void) {
	int	status = listen(this->_fd, 5);
	if (status == -1) {
		throw (FatalException("System call to listen in SocketListen::listen_on_socket()\n"));
	}
	get_port_number_from_socket_fd();
}

int	SocketListen::get_port_number_from_socket_fd(void) const {
	struct sockaddr_in	local_sin;
	socklen_t			local_sin_len = sizeof(local_sin);
	
	if (getsockname(this->_fd, (struct sockaddr *)&local_sin, &local_sin_len) != -1)
		std::cout << "Socket is listening on port " << ntohs(local_sin.sin_port) << " with fd [" << this->_fd << "]\n";
	return (ntohs(local_sin.sin_port));
}

struct sockaddr_in const&	SocketListen::get_addr(void) const {
	return (this->_addr);
}

int const&	SocketListen::get_fd(void) const {
	return (this->_fd);
}

int const&	SocketListen::get_port_number(void) const {
	return (this->_port_number);
}

SocketListen::SocketListen() {

}
