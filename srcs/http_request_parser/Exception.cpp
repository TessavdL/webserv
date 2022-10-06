#include "Exception.hpp"

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
