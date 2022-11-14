/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_data.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 13:50:20 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 14:00:02 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_data.hpp"

ResponseData::ResponseData() {

}

ResponseData::~ResponseData() {

}

ResponseData::ResponseData(ResponseData const& other) {
	*this = other;
}

ResponseData&	ResponseData::operator=(ResponseData const& other) {
	if (this != &other) {
		this->_body = other._body;
		this->_headers = other._headers;
		this->_reason_phrase = other._reason_phrase;
		this->_status_code = other._status_code;
	}
	return (*this);
}

std::string const&					ResponseData::get_body(void) const {
	return (this->_body);
}

std::map<std::string, std::string>	ResponseData::get_headers(void) const {
	return (this->_headers);
}

std::string const&					ResponseData::get_reason_phrase(void) const {
	return (this->_reason_phrase);
}

int const&							ResponseData::get_status_code(void) const {
	return (this->_status_code);
}

void								ResponseData::set_body(std::string const& body) {
	this->_body = body;
}

void								ResponseData::set_headers(std::map<std::string, std::string> const& headers) {
	this->_headers = headers;
}

void								ResponseData::set_reason_phrase(std::string const& reason_phrase) {
	this->_reason_phrase = reason_phrase;
}

void								ResponseData::set_status_code(int const& status_code) {
	this->_status_code = status_code;
}	
