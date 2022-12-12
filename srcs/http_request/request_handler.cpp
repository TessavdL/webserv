/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:43:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/08 13:05:29 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/request_handler.hpp"

RequestHandler::RequestHandler(void) {
	this->_state = REQUEST_START;
}

RequestHandler::~RequestHandler(void) {

}

RequestHandler::RequestHandler(RequestHandler const& other) {
	*this = other;
}

RequestHandler&	RequestHandler::operator=(RequestHandler const& other) {
	if (this != &other) {
		this->_remainder = other._remainder;
		this->_request_line_full = other._request_line_full;
		this->_request_line_method = other._request_line_method;
		this->_request_line_uri = other._request_line_uri;
		this->_request_line_uri_raw = other._request_line_uri_raw;
		this->_request_line_protocol = other._request_line_protocol;
		this->_request_headers_full = other._request_headers_full;
		this->_request_headers_tokens = other._request_headers_tokens;
		this->_request_headers = other._request_headers;
		this->_request_body = other._request_body;
		this->_state = other._state;
	}
	return (*this);
}

void	RequestHandler::process_request(std::string const& request) {
	if (this->_state == REQUEST_START) {
		process_request_start(request);
	}

	std::string 	str = this->_remainder.append(request);
	size_t			index = 0;

	switch (this->_state) {
		case REQUEST_START:
			break ;
		case REQUEST_LINE:
			this->_remainder.clear();
			handle_request_line(str, index);
		case REQUEST_HEADERS:
			handle_headers(str, index);
		case REQUEST_CHECK:
			check_request();
		case REQUEST_BODY:
			handle_body(str, index);
			break ;
		default:
			throw (RequestException(400, "RequestHandler::process_request"));
			return ;
	}
}

void	RequestHandler::process_request_start(std::string const& request) {
	size_t 		pos = request.find(DOUBLE_CRLF);

	if (pos == std::string::npos) {
		this->_remainder.append(request);
	}
	else {
		this->_state = REQUEST_LINE;
	}
}

// HANDLE REQUEST LINE

void	RequestHandler::handle_request_line(std::string const& str, size_t& index) {
	full_request_line(str, index);
	tokenize_request_line(this->_request_line_full);
	this->_request_line_uri.parse_uri(this->_request_line_uri_raw);
	if (this->_request_line_uri.get_path_full().length() > MAX_URI_SIZE) {
		throw (RequestException(414, "RequestHandler::handle_request_line"));
	}
	this->_state = REQUEST_HEADERS;
}

void	RequestHandler::full_request_line(std::string const& str, size_t& index) {
	std::string	request_line_full = string_until_deilimeter(str, CRLF);
	if (!request_line_full.empty()) {
		index += request_line_full.length() + 2;
		this->_request_line_full = request_line_full;
	}
	else {
		throw (RequestException(400, "RequestHandler::full_request_line"));
	}
}

void	RequestHandler::tokenize_request_line(std::string const& str) {
	std::vector<std::string>	request_line = split_string_on_delimeter(str, ' ');
	
	if (!request_line.empty() && request_line.size() == 3) {
		this->_request_line_method = request_line[0];
		this->_request_line_uri_raw = request_line[1];
		this->_request_line_protocol = request_line[2];
	}
	else {
		throw (RequestException(400, "RequestHandler::tokenize_request_line"));
	}
}

// HANDLE HEADERS

void	RequestHandler::handle_headers(std::string const& str, size_t& index) {
	full_headers(str, index);
	if (this->_request_headers_full.length() > MAX_HEADER_SIZE) {
		throw (RequestException(431, "RequestHandler::handle_headers"));
	}
	tokenize_request_headers(this->_request_headers_full);
	create_headers_map(this->_request_headers_tokens);
	this->_state = REQUEST_CHECK;
}

void	RequestHandler::full_headers(std::string const& str, size_t& index) {
	std::string request_headers_full = string_until_deilimeter(str.substr(index), DOUBLE_CRLF);
	
	if (!request_headers_full.empty()) {
		index += request_headers_full.length() + 4;
		this->_request_headers_full = request_headers_full;
	}
	else {
		throw (RequestException(400, "RequestHandler::full_headers()"));
	}
}

void	RequestHandler::tokenize_request_headers(std::string const& str) {
	std::vector<std::string>	request_headers_tokens = split_string_on_delimeter_string(str, CRLF);
	
	if (!request_headers_tokens.empty()) {
		this->_request_headers_tokens = request_headers_tokens;
	}
	else {
		throw (RequestException(400, "RequestHandler::tokenize_request_headers"));
	}
}

void	RequestHandler::create_headers_map(std::vector<std::string> const& v) {
	std::map<std::string, std::string>	request_headers;

	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); it++) {
		std::pair<std::string, std::string>	header_key_and_value = split_string_in_half(*it, ": ");
		if (header_key_and_value.first.empty() || header_key_and_value.second.empty() || request_headers.find(header_key_and_value.first) != request_headers.end()) {
			throw (RequestException(400, "RequestHandler::create_headers_map"));
			return ;
		}
		request_headers[header_key_and_value.first] = header_key_and_value.second;
	}
	this->_request_headers = request_headers;
}

// CHECK REQUEST

void	RequestHandler::check_request(void) {
	std::map<std::string, std::string>::const_iterator	expect_header = this->_request_headers.find("Expect");
	
	if (expect_header != this->_request_headers.end()) {
		std::string const	expectation = string_to_lower(expect_header->second);
		if (!expectation.compare("100-continue")) {
			throw (RequestException(100, "RequestHandler::check_request"));
		}
		else {
			throw (RequestException(417, "RequestHandler::check_request"));
		}
	}
	this->_state = REQUEST_BODY;
}

// HANDLE BODY

static bool	is_request_chunked(std::map<std::string, std::string> const& m) {
	std::map<std::string, std::string>::const_iterator it = m.find("Transfer-Encoding");
	
	if (it != m.end() && !it->second.compare("chunked")) {
		return (true);
	}
	return (false);
}

void	RequestHandler::handle_body(std::string const& str, size_t& index) {
	if (is_request_chunked(this->_request_headers) == true) {
		handle_chunked_body(str, index);
	}
	else {
		handle_normal_body(str, index);
	}
}

void	RequestHandler::handle_normal_body(std::string const& str, size_t& index) {
	this->_request_body.append(str.substr(index));
	this->_remainder.clear();
}

void	RequestHandler::handle_chunked_body(std::string const& str, size_t &index) {
	// std::string substring = str.substr(index);
	// bool		is_done = false;
	// if (substring.find(DOUBLE_CRLF)) {
	// 	is_done = true;
	// }
	// size_t		pos = substring.find(CRLF);
	// bool		hex = true;
	// size_t		n;

	// for (; pos != std::string::npos;) {
	// 	if (hex == true) {
	// 		n = to_dec(substring);
	// 		hex = false;
	// 	}
	// 	else {
	// 		std::string s = substring.substr(0, pos);
	// 		std::cout << s << std::endl;
	// 		if (s.length() == n) {
	// 			this->_request_body.append(s);
	// 			if (n == 0) {
	// 				if (is_done == true)
	// 					break ;
	// 				else
	// 					throw (RequestException(400));
	// 			}
	// 		}
	// 		else {
	// 			throw (RequestException(400));
	// 		}
	// 		hex = true;
	// 	}
	// 	substring = substring.substr(pos + 2);
	// 	pos = substring.find(CRLF);
	// }
	// this->_remainder.clear();
	// this->_remainder = substring;
	chunked_request(str.substr(index), this->_remainder);
}

// GETTERS

RequestHandler::State const&	RequestHandler::get_state(void) const {
	return (this->_state);
}

std::string	const&	RequestHandler::get_remainder(void) const {
	return (this->_remainder);
}

std::string	const&	RequestHandler::get_request_line_method(void) const {
	return (this->_request_line_method);
}

std::string	const&	RequestHandler::get_request_line_uri_raw(void) const {
	return (this->_request_line_uri_raw);
}

Uri	const&	RequestHandler::get_request_line_uri(void) const {
	return (this->_request_line_uri);
}

std::string const&	RequestHandler::get_request_line_protocol(void) const {
	return (this->_request_line_protocol);
}

std::map<std::string, std::string>	const&	RequestHandler::get_headers(void) const {
	return (this->_request_headers);
}

std::string	const&	RequestHandler::get_body(void) const {
	return (this->_request_body);
}

std::ostream&	operator<<(std::ostream& os, RequestHandler const& lexer) {
	if (!lexer.get_request_line_method().empty()) {
		os << "method = " << lexer.get_request_line_method() << std::endl;
	}
	os << lexer.get_request_line_uri();
	if (!lexer.get_request_line_protocol().empty()) {
		os << "protocol = " << lexer.get_request_line_protocol() << std::endl;
	}
	if (!lexer.get_headers().empty()) {
		os << "headers = " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = lexer.get_headers().begin(); it != lexer.get_headers().end(); it++) {
			os << "\t" << it->first << ": " << it->second << std::endl;
		}
	}
	if (!lexer.get_body().empty()) {
		os << "body = " << std::endl << lexer.get_body() << std::endl;
	}
	return (os);
}
