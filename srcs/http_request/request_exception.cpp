/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_exception.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:57:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/07 14:30:07 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/request_exception.hpp"

RequestException::RequestException(int const& status_code, std::string const& function_name) : _status_code(status_code), _message("An error occured in " + function_name) {}

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

int const& RequestException::get_status_code(void) const {
	return (this->_status_code);
}

RequestException::RequestException(void) {}
