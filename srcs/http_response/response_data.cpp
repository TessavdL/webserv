/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_data.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 13:50:20 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 18:51:07 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_data.hpp"

ResponseData::ResponseData() {
	this->_bytes_in_data = 0;
	this->_total_bytes_read = 0;
	this->_total_bytes_sent = 0;
	this->_status_code = 0;
	this->_generated = false;
}

ResponseData::~ResponseData() {

}

ResponseData::ResponseData(ResponseData const& other) {
	*this = other;
}

ResponseData&	ResponseData::operator=(ResponseData const& other) {
	if (this != &other) {
		this->_bytes_in_data = other._bytes_in_data;
		this->_total_bytes_read = other._total_bytes_read;
		this->_total_bytes_sent = other._total_bytes_sent;
		this->_status_code = other._status_code;
		this->_reason_phrase = other._reason_phrase;
		this->_headers = other._headers;
		this->_body = other._body;
		this->_generated = other._generated;
		this->_cgi_response = other._cgi_response;
		this->_response_string = other._response_string;
	}
	return (*this);
}

int	const&	ResponseData::get_bytes_in_data(void) const {
	return (this->_bytes_in_data);
}

int	const&	ResponseData::get_total_bytes_read(void) const {
	return (this->_total_bytes_read);
}

int	const&	ResponseData::get_total_bytes_sent(void) const {
	return (this->_total_bytes_sent);
}

int const&	ResponseData::get_status_code(void) const {
	return (this->_status_code);
}

std::string const&	ResponseData::get_reason_phrase(void) const {
	return (this->_reason_phrase);
}

std::map<std::string, std::string> const&	ResponseData::get_headers(void) const {
	return (this->_headers);
}

std::string const&	ResponseData::get_body(void) const {
	return (this->_body);
}

bool const&	ResponseData::get_generated(void) const {
	return (this->_generated);
}

std::string	const &	ResponseData::get_cgi_response(void) const {
	return (this->_cgi_response);
}

std::string const&	ResponseData::get_repsonse_string(void) const {
	return (this->_response_string);
}

void	ResponseData::set_bytes_in_data(int const bytes_in_data) {
	this->_bytes_in_data = bytes_in_data;
}

void	ResponseData::set_total_bytes_read(int const total_bytes_read) {
	this->_total_bytes_read = total_bytes_read;
}

void	ResponseData::set_total_bytes_sent(int const total_bytes_sent) {
	this->_total_bytes_sent = total_bytes_sent;
}

void	ResponseData::set_status_code(int const status_code) {
	this->_status_code = status_code;
}

void	ResponseData::set_reason_phrase(std::string const& reason_phrase) {
	this->_reason_phrase = reason_phrase;
}

void	ResponseData::set_headers(std::map<std::string, std::string> const& headers) {
	this->_headers = headers;
}

void	ResponseData::set_body(std::string const& body) {
	this->_body = body;
}

void	ResponseData::set_generated(bool const generated) {
	this->_generated = generated;
}

void	ResponseData::set_cgi_response(std::string const& cgi_response) {
	this->_cgi_response = cgi_response;
}

void	ResponseData::set_response_string(std::string const& response_string) {
	this->_response_string = response_string;
}
