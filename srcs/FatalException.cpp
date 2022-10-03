#include "FatalException.hpp"

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
