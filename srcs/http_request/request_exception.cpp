/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_exception.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:57:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 13:57:13 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/request_exception.hpp"

RequestException::RequestException(void) {
	this->_message = "HTTP Request is invalid";
}

RequestException::RequestException(std::string const message) : _message(message) {}	

RequestException::RequestException(RequestException const& src) : _message(src._message) {}

RequestException::~RequestException(void) throw() {}

RequestException&	RequestException::operator=(RequestException const& other) {
	if (this != &other) {
		this->_message = other._message;
	}
	return (*this);
}

char const* RequestException::what(void) const throw() {
	return (this->_message.c_str());
}
