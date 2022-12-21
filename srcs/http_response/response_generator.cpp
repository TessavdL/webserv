/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_generator.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 17:29:52 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/21 16:36:53 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_generator.hpp"

#define HTTP_VERSION "HTTP/1.1"
#define HEADER_SEPERATOR ": "
#define CRLF "\r\n"

ResponseGenerator::ResponseGenerator(void) {

}

ResponseGenerator::~ResponseGenerator(void) {

}

ResponseGenerator::ResponseGenerator(ResponseGenerator const& other) {
	*this = other;
}

ResponseGenerator&	ResponseGenerator::operator=(ResponseGenerator const& other) {
	if (this != &other) {
		this->_body = other._body;
		this->_full_response = other._full_response;
		this->_headers = other._headers;
		this->_status_line = other._status_line;
	}
	return (*this);
}

void	ResponseGenerator::generate_response_string(ResponseData response) {
	// STATUS LINE
	this->_status_line.append(HTTP_VERSION);
	this->_status_line.append(" ");
	this->_status_line.append(std::to_string(response.get_status_code()));
	this->_status_line.append(" ");
	this->_status_line.append(response.get_reason_phrase());
	this->_status_line.append(CRLF);

	// HEADERS
	this->_headers.append("Date");
	this->_headers.append(HEADER_SEPERATOR);
	this->_headers.append(get_date_information());
	this->_headers.append(CRLF);
	for (std::map<std::string, std::string>::const_iterator it = response.get_headers().begin(); it != response.get_headers().end(); it++) {
		this->_headers.append(it->first);
		this->_headers.append(HEADER_SEPERATOR);
		this->_headers.append(it->second);
		this->_headers.append(CRLF);
	}
	this->_headers.append(CRLF);

	// BODY
	this->_body.append(response.get_body());

	// FULL RESPONSE	
	this->_full_response.append(this->_status_line);
	this->_full_response.append(this->_headers);
	this->_full_response.append(this->_body);
}

std::string const&	ResponseGenerator::get_body(void) const {
	return (this->_body);
}

std::string const&	ResponseGenerator::get_full_response(void) const {
	return (this->_full_response);
}

std::string const&	ResponseGenerator::get_headers(void) const {
	return (this->_headers);
}

std::string const&	ResponseGenerator::get_status_line(void) const {
	return (this->_status_line);
}

void	ResponseGenerator::set_body(std::string const& body) {
	this->_body = body;
}

void	ResponseGenerator::set_full_response(std::string const& full_response) {
	this->_full_response = full_response;
}

void	ResponseGenerator::set_headers(std::string const& headers) {
	this->_headers = headers;
}

void	ResponseGenerator::set_status_line(std::string const& status_line) {
	this->_status_line = status_line;
}

std::ostream&	operator<<(std::ostream& os, ResponseGenerator const& response) {
	os << "---STATUS LINE---" << std::endl << response.get_status_line() << std::endl;
	os << "---HEADERS---" << std::endl << response.get_headers() << std::endl;
	return (os);
}
