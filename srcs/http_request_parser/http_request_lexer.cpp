/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_request_lexer.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:35:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/02 17:24:11 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/http_request_lexer.hpp"

#include <iostream>
#include <sstream>

HTTPRequestLexer::HTTPRequestLexer(void) {
	this->_state = REQUEST_START;
}

HTTPRequestLexer::~HTTPRequestLexer(void) {

}

HTTPRequestLexer::HTTPRequestLexer(HTTPRequestLexer const& other) {
	*this = other;
}

// uri struct
HTTPRequestLexer&	HTTPRequestLexer::operator=(HTTPRequestLexer const& other) {
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
	}
	return (*this);
}

void	HTTPRequestLexer::full_request_line(std::string const& str, size_t& index) {
	std::string	request_line_full = string_until_deilimeter(str, CLRF);
	if (!request_line_full.empty()) {
		index += request_line_full.length() + 2;
		this->_request_line_full = request_line_full;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void	HTTPRequestLexer::tokenize_request_line(std::string const& str) {
	std::vector<std::string>	request_line = split_string_on_delimeter(str, ' ');
	
	if (!request_line.empty()) {
		this->_request_line_method = request_line[0];
		this->_request_line_uri_raw = request_line[1];
		this->_request_line_protocol = request_line[2];
		this->_state = REQUEST_HEADERS;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void	HTTPRequestLexer::full_headers(std::string const& str, size_t& index) {
	std::string request_headers_full = string_until_deilimeter(str.substr(index), DOUBLE_CLRF);
	
	if (!request_headers_full.empty()) {
		index += request_headers_full.length() + 4;
		this->_request_headers_full = request_headers_full;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void	HTTPRequestLexer::tokenize_request_headers(std::string const& str) {
	std::vector<std::string>	request_headers_tokens = split_string_on_delimeter_string(str, CLRF);
	
	if (!request_headers_tokens.empty()) {
		this->_request_headers_tokens = request_headers_tokens;
	}
	else {
		this->_state = REQUEST_ERROR;
	}
}

void	HTTPRequestLexer::create_headers_map(std::vector<std::string> const& v) {
	std::map<std::string, std::string>	request_headers;

	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); it++) {
		std::pair<std::string, std::string>	header_key_and_value = split_string_in_half(*it, ": ");
		if (header_key_and_value.first.empty() || header_key_and_value.second.empty() || request_headers.find(header_key_and_value.first) != request_headers.end()) {
			this->_state = REQUEST_ERROR;
			return ;
		}
		request_headers[header_key_and_value.first] = header_key_and_value.second;
	}
	this->_request_headers = request_headers;
	// for (std::map<std::string, std::string>::iterator it = this->_request_headers.begin(); it != this->_request_headers.end(); it++) {
	// 	std::cout << it->first << " " << it->second << std::endl;
	// }
	this->_state = REQUEST_BODY;
}

static bool	is_request_chunked(std::map<std::string, std::string> const& m) {
	std::map<std::string, std::string>::const_iterator it = m.find("Transfer-Encoding");
	
	if (it != m.end() && it->second == "chunked") {
		return (true);
	}
	return (false);
}

void	HTTPRequestLexer::process_request(std::string const& request) {
	if (this->_state == REQUEST_START) {
		process_request_start(request);
	}

	std::string str = this->_remainder.append(request);
	size_t		index = 0;

	switch (this->_state) {
		case REQUEST_START:
			break ;
		case REQUEST_LINE:
			full_request_line(str, index);
			tokenize_request_line(this->_request_line_full);
			this->_request_line_uri.parse_uri(this->_request_line_uri_raw);
		case REQUEST_HEADERS:
			full_headers(str, index);
			tokenize_request_headers(this->_request_headers_full);
			create_headers_map(this->_request_headers_tokens);
		case REQUEST_BODY:
			if (is_request_chunked(this->_request_headers) == true) {
				go_chonky_body(str, index);
			}
			else {
				go_body(str, index);
			}	
			break ;
		default:
			std::cout << "ERROR IN HTTP REQUEST" << std::endl;
			return ;
	}
}

void	HTTPRequestLexer::process_request_start(std::string const& request) {
	size_t 		pos = request.find(DOUBLE_CLRF);
	std::string	str;

	if (pos == std::string::npos) {
		this->_remainder.append(request);
	}
	else {
		this->_state = REQUEST_LINE;
	}
}

// void	HTTPRequestLexer::go_method(std::string const& str, size_t& index) {
// 	size_t	pos = str.find(" ");

// 	if (pos != std::string::npos) {
// 		this->_request_line.method= str.substr(0, pos);
// 		index = pos + 1;
// 		this->_state = REQUEST_LINE_URI;
// 	}
// 	else {
// 		this->_state = REQUEST_ERROR;
// 	}
// }

// void				HTTPRequestLexer::go_uri(std::string const& str, size_t& index) {
// 	std::string remainder = str.substr(index, str.size());
// 	size_t	pos = remainder.find(" ");

// 	if (pos != std::string::npos) {
// 		this->_request_line.uri = remainder.substr(0, pos);
// 		index += (pos + 1);
// 		this->_state = REQUEST_LINE_PROTOCOL;
// 	}
// 	else {
// 		this->_state = REQUEST_ERROR;
// 	}
// }

// void	HTTPRequestLexer::go_protocol(std::string const& str, size_t& index) {
// 	std::string remainder = str.substr(index, str.size());
// 	size_t	pos = remainder.find(CLRF);

// 	if (pos != std::string::npos) {
// 		this->_request_line.protocol = remainder.substr(0, pos);
// 		index += (pos + 2);
// 		this->_state = REQUEST_HEADERS;
// 	}
// 	else {
// 		this->_state = REQUEST_ERROR;
// 	}
// }

// void	HTTPRequestLexer::go_headers(std::string const& str, size_t& index) {
// 	size_t		end = str.find(DOUBLE_CLRF);
// 	std::string remainder = str.substr(index, end - index);

// 	size_t pos = remainder.find(CLRF);
// 	for (;pos != std::string::npos;) {
// 		this->_request_headers.push_back(remainder.substr(0, pos));
// 		remainder = remainder.substr(pos + 2, remainder.size());
// 		pos = remainder.find(CLRF);
// 	}
// 	this->_request_headers.push_back(remainder);
// 	index = end + 4;
// 	this->_state = REQUEST_BODY;
// }

void	HTTPRequestLexer::go_body(std::string const& str, size_t& index) {
	this->_request_body = str.substr(index);
}

static int to_dec(std::string hex) {
	unsigned int 		x;   
	std::stringstream	ss;

	ss << std::hex << hex;
	ss >> x;
	return (static_cast<int>(x));
}

void	HTTPRequestLexer::go_chonky_body(std::string const& str, size_t &index) {
	std::string substring = str.substr(index);
	size_t		pos = substring.find(CLRF);
	bool		hex = true;
	size_t		n;

	for (; pos != std::string::npos;) {
		if (hex == true) {
			n = to_dec(substring);
			// std::cout << "number of bytes in next chonk = " << n << std::endl;
			if (n == 0) {
				// std::cout << "END" << std::endl;
				break ;
			}
			hex = false;
		}
		else {
			std::string s = substring.substr(0, pos);
			std::cout << s << std::endl;
			if (s.length() == n) {
				// std::cout << "OK" << std::endl;
				this->_request_body.append(s);
			}
			else {
				this->_state = REQUEST_ERROR;
			}
			hex = true;
		}
		substring = substring.substr(pos + 2);
		pos = substring.find(CLRF);
	}
	// std::cout << "END OF CHONKY BODY" << std::endl;
}

HTTPRequestLexer::State const&	HTTPRequestLexer::get_state(void) const {
	return (this->_state);
}

std::string	const&	HTTPRequestLexer::get_remainder(void) const {
	return (this->_remainder);
}

std::string	const&	HTTPRequestLexer::get_request_line_method(void) const {
	return (this->_request_line_method);
}

std::string	const&	HTTPRequestLexer::get_request_line_uri_raw(void) const {
	return (this->_request_line_uri_raw);
}

Uri	const&	HTTPRequestLexer::get_request_line_uri(void) const {
	return (this->_request_line_uri);
}

std::string const&	HTTPRequestLexer::get_request_line_protocol(void) const {
	return (this->_request_line_protocol);
}

std::map<std::string, std::string>	const&	HTTPRequestLexer::get_headers(void) const {
	return (this->_request_headers);
}

std::string	const&	HTTPRequestLexer::get_body(void) const {
	return (this->_request_body);
}

std::ostream&	operator<<(std::ostream& os, HTTPRequestLexer const& lexer) {
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
