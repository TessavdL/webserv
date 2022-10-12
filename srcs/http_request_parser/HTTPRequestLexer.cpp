#include "HTTPRequestLexer.hpp"

#include <iostream>

HTTPRequestLexer::HTTPRequestLexer(void) {
	this->_state = REQUEST_START;
	this->parser = new HTTPRequestParser();
}

HTTPRequestLexer::~HTTPRequestLexer(void) {
	delete this->parser;
}

HTTPRequestLexer::HTTPRequestLexer(HTTPRequestLexer const& other) {
	*this = other;
}

HTTPRequestLexer&	HTTPRequestLexer::operator=(HTTPRequestLexer const& other) {
	if (this != &other) {
		this->_remainder = other._remainder;
		this->_request_line = other._request_line;
		this->_request_headers = other._request_headers;
		this->_request_body = other._request_body;
	}
	return (*this);
}

void				HTTPRequestLexer::process_request(std::string const& request) {
	if (this->_state == REQUEST_START) {
		process_request_start(request);
	}

	std::string str = this->_remainder.append(request);
	size_t		index = 0;

	switch (this->_state) {
		case REQUEST_START:
			break ;
		case REQUEST_LINE_METHOD:
			go_method(str, index);
			this->parser->check_method(this->_request_line.method);
		case REQUEST_LINE_URI:
			go_uri(str, index);
			this->parser->check_uri(this->_request_line.uri);
		case REQUEST_LINE_PROTOCOL:
			go_protocol(str, index);
		case REQUEST_HEADERS:
			go_headers(str, index);
			this->parser->check_headers(this->_request_headers);
		case REQUEST_BODY:
			go_body(str, index);
			return ;
		default:
			std::cout << "ERROR IN HTTP REQUEST" << std::endl;
			return ;
	}
}

void				HTTPRequestLexer::process_request_start(std::string const& request) {
	size_t 		pos = request.find(DOUBLE_CLRF);
	std::string	str;

	if (pos == std::string::npos) {
		this->_remainder.append(request);
	}
	else {
		this->_state = REQUEST_LINE_METHOD;
	}
}

void				HTTPRequestLexer::go_method(std::string const& str, size_t& index) {
	size_t	pos = str.find(" ");

	if (pos != std::string::npos) {
		this->_request_line.method= str.substr(0, pos);
		// std::cout << "Method = " << this->_request_line.method << std::endl;
		index = pos + 1;
		this->_state = REQUEST_LINE_URI;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void				HTTPRequestLexer::go_uri(std::string const& str, size_t& index) {
	std::string remainder = str.substr(index, str.size());
	size_t	pos = remainder.find(" ");

	if (pos != std::string::npos) {
		this->_request_line.uri = remainder.substr(0, pos);
		// std::cout << "URI = " << this->_request_line.uri << std::endl;
		index += (pos + 1);
		this->_state = REQUEST_LINE_PROTOCOL;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void				HTTPRequestLexer::go_protocol(std::string const& str, size_t& index) {
	std::string remainder = str.substr(index, str.size());
	size_t	pos = remainder.find(CLRF);

	if (pos != std::string::npos) {
		this->_request_line.protocol = remainder.substr(0, pos);
		// std::cout << "PROTOCOL = " << this->_request_line.protocol << std::endl;
		index += (pos + 2);
		this->_state = REQUEST_HEADERS;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void				HTTPRequestLexer::go_headers(std::string const& str, size_t& index) {
	size_t		end = str.find(DOUBLE_CLRF);
	std::string remainder = str.substr(index, end - index);

	size_t pos = remainder.find(CLRF);
	for (;pos != std::string::npos;) {
		this->_request_headers.push_back(remainder.substr(0, pos));
		// std::cout << "HEADER = " << this->_request_headers.back() << std::endl;
		remainder = remainder.substr(pos + 2, remainder.size());
		pos = remainder.find(CLRF);
	}
	this->_request_headers.push_back(remainder);
	// std::cout << "HEADER = " << this->_request_headers.back() << std::endl;
	index = end + 4;
	this->_state = REQUEST_BODY;
}

void				HTTPRequestLexer::go_body(std::string const& str, size_t& index) {
	this->_request_body = str.substr(index, str.size());
}

HTTPRequestLexer::State				HTTPRequestLexer::get_state(void) const {
	return (this->_state);
}

std::string						HTTPRequestLexer::get_remainder(void) const {
	return (this->_remainder);
}

HTTPRequestLexer::RequestLine	HTTPRequestLexer::get_request_line(void) const {
	return (this->_request_line);
}

std::vector<std::string>		HTTPRequestLexer::get_headers(void) const {
	return (this->_request_headers);
}

std::string						HTTPRequestLexer::get_body(void) const {
	return (this->_request_body);
}


std::ostream&	operator<<(std::ostream& os, HTTPRequestLexer const& lexer) {
	if (!lexer.get_request_line().method.empty()) {
		os << "METHOD = " << lexer.get_request_line().method << std::endl;
	}
	if (!lexer.get_request_line().uri.empty()) {
		os << "URI = " << lexer.get_request_line().uri << std::endl;
	}
	if (!lexer.get_request_line().protocol.empty()) {
		os << "PROTOCOL = " << lexer.get_request_line().protocol << std::endl;
	}
	if (!lexer.get_headers().empty()) {
		for (size_t i = 0; i < lexer.get_headers().size(); i++) {
			os << "HEADER = " << lexer.get_headers()[i] << std::endl;
		}
	}
	if (!lexer.get_body().empty()) {
		os << "BODY = " << lexer.get_body() << std::endl;
	}
	return (os);
}
