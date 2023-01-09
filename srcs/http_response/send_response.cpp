/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   send_response.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 14:23:30 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 16:25:34 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/send_data.hpp"

SendData::SendData(std::string const& response_string) {
	this->_total_bytes_sent = 0;
	this->_size = 0;
	this->_response_string = response_string;
}

SendData::~SendData(void) {

}

SendData::SendData(SendData const& other) {
	*this = other;
}

SendData&	SendData::operator=(SendData const& other) {
	if (this != &other) {
		this->_size = other._size;
		this->_total_bytes_sent = other._total_bytes_sent;
		this->_response_string = other._response_string;
	}
	return (*this);
}

void	SendData::add_bytes_sent_to_total_bytes_sent(int const& bytes_sent) {
	this->_total_bytes_sent += bytes_sent;
}

ssize_t const&	SendData::get_total_bytes_sent(void) const {
	return (this->_total_bytes_sent);
}

void	SendData::set_total_bytes_sent(int const total_bytes_sent) {
	this->_total_bytes_sent = total_bytes_sent;
}

SendData::SendData(void) {

}
