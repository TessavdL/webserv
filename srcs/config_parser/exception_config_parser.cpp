/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_config_parser.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:45:30 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 17:45:41 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/exception_config_parser.hpp"

LexerParserException::LexerParserException(void) {
	this->_message = "Config file is not valid";
}

LexerParserException::LexerParserException(std::string const message) : _message(message) {}	

LexerParserException::LexerParserException(LexerParserException const& src) : _message(src._message) {}

LexerParserException::~LexerParserException(void) throw() {}

LexerParserException&	LexerParserException::operator=(LexerParserException const& other) {
	if (this != &other) {
		this->_message = other._message;
	}
	return (*this);
}

char const* LexerParserException::what(void) const throw() {
	return (this->_message.c_str());
}