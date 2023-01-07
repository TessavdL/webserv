/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_config_parser.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:45:30 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:39:05 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/exception_config_parser.hpp"

ConfigException::ConfigException(void) {
	this->_message = "Config file is not valid";
}

ConfigException::ConfigException(std::string const message) : _message(message) {}	

ConfigException::ConfigException(ConfigException const& src) : _message(src._message) {}

ConfigException::~ConfigException(void) throw() {}

ConfigException&	ConfigException::operator=(ConfigException const& other) {
	if (this != &other) {
		this->_message = other._message;
	}
	return (*this);
}

char const* ConfigException::what(void) const throw() {
	return (this->_message.c_str());
}
