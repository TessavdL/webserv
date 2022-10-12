/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_http_request_parser.cpp                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:36:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 17:39:28 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/exception_http_request_parser.hpp"

HTTPRequestException::HTTPRequestException(void) {
	this->_message = "HTTP Request is invalid";
}

HTTPRequestException::HTTPRequestException(std::string const message) : _message(message) {}	

HTTPRequestException::HTTPRequestException(HTTPRequestException const& src) : _message(src._message) {}

HTTPRequestException::~HTTPRequestException(void) throw() {}

HTTPRequestException&	HTTPRequestException::operator=(HTTPRequestException const& other) {
	if (this != &other) {
		this->_message = other._message;
	}
	return (*this);
}

char const* HTTPRequestException::what(void) const throw() {
	return (this->_message.c_str());
}
