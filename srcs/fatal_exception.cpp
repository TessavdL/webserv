/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fatal_exception.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 15:04:18 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:08:32 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fatal_exception.hpp"

FatalException::FatalException(void) {
	this->_message.append("A fatal error has occured\n");
	this->_message.append(strerror(errno));
}

FatalException::~FatalException(void) throw() {}

FatalException::FatalException(std::string const error_information) {
	this->_message.append("A fatal error has occured\n");
	this->_message.append(error_information);
	this->_message.append(strerror(errno));
}

FatalException::FatalException(FatalException const& src) : _message(src._message) {}

FatalException&	FatalException::operator=(FatalException const& other) {
	if (this != &other) {
		this->_message = other._message;
	}
	return (*this);
}

char const* FatalException::what(void) const throw() {
	return (this->_message.c_str());
}
