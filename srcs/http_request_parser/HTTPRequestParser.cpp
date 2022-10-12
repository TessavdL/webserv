/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   HTTPRequestParser.cpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:36:21 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/12 17:38:44 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"

#include <iostream>

HTTPRequestParser::HTTPRequestParser(void) {

}

// HTTPRequestParser::HTTPRequestParser(std::string const& request) {

// }

HTTPRequestParser::~HTTPRequestParser(void) {

}

HTTPRequestParser::HTTPRequestParser(HTTPRequestParser const& other) {
	*this = other;
}

HTTPRequestParser&	HTTPRequestParser::operator=(HTTPRequestParser const& other) {
	if (this != &other) {

	}
	return (*this);
}

// Needs information from server if methods are limited
Method	HTTPRequestParser::check_method(std::string const& str) {
	const char *method_types[3] = {
		"GET",
		"POST",
		"DELETE"
	};

	for (int i = 0; i < 3; i++) {
		if (str == std::string(method_types[i])) {
			this->_method = std::string(method_types[i]);
			return (static_cast<Method>(i));
		}
	}
	return (INVALID);
}

void	HTTPRequestParser::check_uri(std::string const& uri) {
	std::string	remainder = uri;
	size_t		pos = remainder.find("://");
	
	if (pos != std::string::npos) {
		this->_uri.scheme = remainder.substr(0, pos);
		std::cout << "URI SCHEME = " << this->_uri.scheme << std::endl;
		remainder = remainder.substr(pos + 3);
	}
	pos = remainder.find("/");
	std::string authority = remainder.substr(0, pos);
	if (pos != std::string::npos && !authority.empty()) {
		size_t	pos2 = authority.find("@");
		if (pos != std::string::npos) {
			this->_uri.authority.user_information = authority.substr(0, pos2);
			std::cout << "URI AUTHORITY USER INFORMATION = " << this->_uri.authority.user_information << std::endl;
			authority = authority.substr(pos + 1);
		}
		pos2 = authority.find(":");
		if (pos2 != std::string::npos) {
			this->_uri.authority.host = authority.substr(0, pos2);
			authority = authority.substr(pos + 1);
			if (!authority.empty())
				this->_uri.authority.port = stoi(authority);
		}
		remainder = remainder.substr(pos + 1);
	}
	else {
		return ;
	}
	pos = remainder.find("?");
	if (pos != std::string::npos && !remainder.empty()) {
		this->_uri.path = remainder.substr(0, pos);
	}
	else {
		pos = remainder.find("#");
		if (pos != std::string::npos && !remainder.empty()) {
			this->_uri.path = remainder.substr(0, pos);
		}
		else {
			this->_uri.path = remainder;
		}
	}
	std::cout << "END OF CHECK URI" << std::endl;	
}

void	HTTPRequestParser::check_headers(std::vector<std::string> const& headers) {
	size_t	pos;

	for (size_t i = 0; i < headers.size(); i++) {
		std::pair<std::string, std::string>	key_value_pair;
		
		std::cout << "headers[i] = " << headers[i] << std::endl;
		pos = headers[i].find(": ");
		if (pos == std::string::npos) {
			std::cout << "ERROR IN CHECK HEADERS" << std::endl;
			break ;
		}
		key_value_pair.first = headers[i].substr(0, pos);
		key_value_pair.second = headers[i].substr(pos + 2, headers[i].size());
		this->_headers.insert(key_value_pair);
	}
	for(std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
		it != this->_headers.end(); ++it)
	{
		std::cout << it->first << " " << it->second << "\n";
		if (it->first == "Transfer-Encoding" && it->second == "chunked") {
			this->_request_body_state = CHUNKED;
		}
	}
}

std::string	HTTPRequestParser::get_method(void) const {
	return (this->_method);
}

Uri	HTTPRequestParser::get_uri(void) const {
	return (this->_uri);
}

std::map<std::string, std::string>	HTTPRequestParser::get_headers(void) const {
	return (this->_headers);
}

std::string	HTTPRequestParser::get_body(void) const {
	return (this->_body);
}

std::string	HTTPRequestParser::get_chonky_body(void) const {
	return (this->_chonky_body);
}

RequestBody	HTTPRequestParser::get_request_body_state(void) const {
	return (this->_request_body_state);
}
