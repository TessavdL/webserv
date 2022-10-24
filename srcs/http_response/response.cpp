/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 15:19:28 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/19 18:52:21 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>

#define HTTP_VERSION "HTTP1/1"
#define CRLF "\r\n"
#define HEADER_SEPERATOR ": "

Response::Response(void) {
	set_status_line();
	set_headers();
	set_body();
	set_full_response();
}

Response::~Response(void) {

}

Response::Response(Response const& other) {
	*this = other;
}

Response&	Response::operator=(Response const& other) {
	if (this != &other) {
		this->_full_response = other._full_response;
		this->_status_line = other._status_line;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
}

void	Response::set_status_line(void) {
	this->_status_line.append(HTTP_VERSION);
	this->_status_line.append(" ");
	this->_status_line.append("200"); // status_code
	this->_status_line.append(" ");
	this->_status_line.append("OK"); // reason_phrase
	this->_status_line.append(CRLF);
}

void	Response::set_headers(void) {
	this->_headers.append("Date");
	this->_headers.append(HEADER_SEPERATOR);
	this->_headers.append(get_date_information());
	this->_headers.append(CRLF);
}

void	Response::set_body(void) {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open("./index.html");
	std::ostringstream ss;
	ss << input_stream.rdbuf();
	input.append(ss.str());
	std::cout << input << std::endl;
	input_stream.close();
	this->_body.append(input);
}

void	Response::set_full_response(void) {
	this->_full_response.append(this->_status_line);
	this->_full_response.append(this->_headers);
	this->_full_response.append(this->_body);
}

std::string const&	Response::get_full_response(void) const {
	return (this->_full_response);
}

std::string const&	Response::get_status_line(void) const {
	return (this->_status_line);
}

std::string const&	Response::get_headers(void) const {
	return (this->_headers);
}

std::string const&	Response::get_body(void) const {
	return (this->_body);
}

std::ostream&	operator<<(std::ostream& os, Response const& response) {
	os << "---STATUS LINE---" << std::endl << response.get_status_line() << std::endl;
	os << "---HEADERS---" << std::endl << response.get_headers() << std::endl;
	if (!response.get_body().empty()) {
		os << "---BODY---" << std::endl << response.get_body() << std::endl;
	}
	return (os);
}
