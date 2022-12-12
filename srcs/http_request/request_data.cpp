/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_data.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 13:24:16 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/08 16:21:19 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/request_data.hpp"

RequestData::RequestData(void) {

}

RequestData::~RequestData(void) {

}

RequestData::RequestData(RequestData const& other) {
	*this = other;
}

RequestData&	RequestData::operator=(RequestData const& other) {
	if (this != &other) {
		this->_bytes_in_data = other._bytes_in_data;
		this->_total_bytes_read = other._total_bytes_read;
		this->_method = other._method;
		this->_uri = other._uri;
		this->_protocol = other._protocol;
		this->_headers = other._headers;
		this->_body = other._body;
	}
	return (*this);
}

std::string const&							RequestData::get_method(void) const {
	return (this->_method);
}

Uri const&									RequestData::get_uri(void) const {
	return (this->_uri);
}

std::string const&							RequestData::get_protocol(void) const {
	return (this->_protocol);
}

std::map<std::string, std::string> const&	RequestData::get_headers(void) const {
	return (this->_headers);
}

std::string const&							RequestData::get_body(void) const {
	return (this->_body);
}

int const&									RequestData::get_bytes_in_data(void) const {
	return (this->_bytes_in_data);
}

int const&									RequestData::get_total_bytes_read(void) const {
	return (this->_total_bytes_read);
}

void										RequestData::set_bytes_in_data(int const& bytes_in_data) {
	this->_bytes_in_data = bytes_in_data;
}

void										RequestData::set_total_bytes_read(int const& total_bytes_read) {
	this->_total_bytes_read = total_bytes_read;
}

void										RequestData::set_method(std::string const& method) {
	this->_method = method;
}							

void										RequestData::set_uri(Uri const& uri) {
	this->_uri = uri;
}					

void										RequestData::set_protocol(std::string const& protocol) {
	this->_protocol = protocol;
}

void										RequestData::set_headers(std::map<std::string, std::string> const& headers) {
	this->_headers = headers;
}

void										RequestData::set_body(std::string const& body) {
	this->_body = body;
}
