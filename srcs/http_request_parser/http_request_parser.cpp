/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_request_parser.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:35:34 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/23 16:31:44 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/http_request_parser.hpp"

#include <iostream>

HTTPRequestParser::HTTPRequestParser(void) {

}

HTTPRequestParser::~HTTPRequestParser(void) {

}

HTTPRequestParser::HTTPRequestParser(HTTPRequestParser const& other) {
	*this = other;
}

HTTPRequestParser&	HTTPRequestParser::operator=(HTTPRequestParser const& other) {
	if (this != &other) {
		this->_method = other._method;
		this->_uri = other._uri;
		this->_headers = other._headers;
		this->_body = other._body;
		this->_chonky_body = other._chonky_body;
		this->_request_body_state = other._request_body_state;
	}
	return (*this);
}

// Needs information from server if methods are limited
Method	HTTPRequestParser::check_method(std::string const& str) {
	const char *method_types[3] = {
		"GET",
		"POST",
		"DELETE"
	};

	for (int i = 0; i < 3; i++) {
		if (str == std::string(method_types[i])) {
			this->_method = std::string(method_types[i]);
			return (static_cast<Method>(i));
		}
	}
	return (INVALID);
}

// 4.2.3. HTTP NORMALIZATION AND COMPARISON
// The scheme and host are case-insensitive and normally provided in lowercase; all other components are compared in a case-sensitive manner.

static void	check_scheme(std::string const& scheme_str, std::string& scheme, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(scheme_str.substr(index), "://");
	scheme = p.first;
	if (!scheme.empty()) {
		std::cout << "SCHEME = " << scheme << std::endl;
		index += scheme.length() + 3;
	}
}

// 4.2.3. HTTP NORMALIZATION AND COMPARISON
// The scheme and host are case-insensitive and normally provided in lowercase; all other components are compared in a case-sensitive manner.
// 4.2.4 Depecrication of userinfo in HTTP URI's
// a recipient SHOULD parse for userinfo and treat its presence as an error; it is likely being used to obscure the authority for the sake of phishing attacks.
static void	check_authority(std::string const& authority_str, Authority& authority, size_t& index) {
	size_t								pos = authority_str.substr(index).find("/");

	if (pos != std::string::npos) {
		index += pos;
	}
	std::pair<std::string, std::string>	p = split_string_in_half(authority_str.substr(index), "/");
	std::string							str = p.first;

	if (!str.empty()) {
		std::pair<std::string, std::string>	p2 = split_string_in_half(str, "@");
		std::string							str2 = p2.first;
		authority.user_information = str2;
		if (!authority.user_information.empty()) {
			// std::cout << "USER INFORMATION = " << authority.user_information << std::endl;
			str2 = p2.second;
		}
		else { // if no @ is found then continue forward with the original string
			str2 = str;
		}
		
		std::pair<std::string, std::string>	p3 = split_string_in_half(str2, ":");
		std::string							port;
		authority.host = p3.first;
		// if (!authority.host.empty()) {
		// 	// std::cout << "AUTHORITY HOST = " << authority.host << std::endl;
		// 	index += (authority.host.length() + 1);
		// }
		// else {
		// 	authority.host = str2;
		// 	if (!authority.host.empty()) {
		// 		// std::cout << "AUTHORITY HOST = " << authority.host << std::endl;
		// 		index += (authority.host.length() + 1);
		// 	}
		// }
		port = p3.second;
		if (!port.empty()) {
			authority.port = stoi(port);
			// index += (port.length() + 1);
			// std::cout << "AUTHORITY PORT = " << authority.port << std::endl;
		}
		else {
			authority.port = -1;
			// std::cout << "AUTHORITY PORT = " << authority.port << std::endl;
		}
	}
}

// HTTP 4.2.3. Normalization and comparison
// When not being used as the target of an OPTIONS request, an empty path component is equivalent to an absolute path of "/", so the normal form is to provide a path of "/" instead.
// Should think of a way to handle multiple "//" after each other
static void	check_path(std::string const& path_str, Path& path, size_t& index) {
	std::string							substring = path_str.substr(index);
	std::pair<std::string, std::string>	p = split_string_in_half_on_any_match(substring, "?#");
	std::string							str = p.first;

	if (!p.first.empty()) {
		path.full = p.first;
		// std::cout << "PATH FULL = " << path.full << std::endl;
	}
	else {
		path.full = substring;
		// std::cout << "PATH FULL = " << path.full << std::endl;
	}
	index += path.full.length();
	std::pair<std::string, std::string>	p2 = split_string_in_half(path.full, ".");
	if (!p2.second.empty()) {
		path.without_extension = p2.first;
		path.extension = p2.second;
		// std::cout << "PATH WITHOUT EXTENSION = " << path.without_extension << std::endl;
		// std::cout << "PATH EXTENSION = " << path.extension << std::endl;
	}
}

static void	check_query(std::string const& query_str, std::map<std::string, std::string>& query, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(query_str.substr(index), "?");
	std::string							str = p.second;

	if (!str.empty()) {
		std::pair<std::string, std::string>	p2 = split_string_in_half(str, "#");
		std::string							str2 = p2.first;
		if (str2.empty()) {
			str2 = str;
			index += str2.length();
		}
		else {
			index += str2.length() + 1;
		}
		std::vector<std::string>	query_seperated = split_string_on_delimeter(str2, '&');
		for (size_t i = 0; i < query_seperated.size(); i++) {
			if (!query_seperated[i].empty()) {
				std::pair<std::string, std::string> p = split_string_in_half(query_seperated[i], "=");
				query.insert(p);
			}
		}
	}
}

static void	check_fragment(std::string const& fragment_str, std::string& fragment, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(fragment_str.substr(index), "#");
	std::string							str = p.second;

	if (!str.empty()) {
		fragment = str;
		// std::cout << "FRAGMENT = " << fragment << std::endl;
		index += fragment.length() + 1;
	}
}

void	HTTPRequestParser::check_uri(std::string const& str) {
	size_t		index = 0;
	
	if (str.length() > 2000) {
		std::cout << "414 URI TOO LONG\n" << std::endl;
		return ;
	}
	// std::cout << "URI = " << str << std::endl;
	check_scheme(str, this->_uri.scheme, index);
	check_authority(str, this->_uri.authority, index);
	check_path(str, this->_uri.path, index);
	if (str.substr(index).empty()) {
		std::cout << std::endl;
		return ;
	}
	check_query(str, this->_uri.query, index);
	if (!this->_uri.query.empty()) {
		// std::cout << "QUERY STRING = ";
		for (std::map<std::string, std::string>::iterator it = this->_uri.query.begin(); it != this->_uri.query.end(); it++) {
			// std::cout << (*it).first << "=" << (*it).second << ",";
		}
		std::cout << std::endl;
	}
	if (str.substr(index).empty()) {
		std::cout << std::endl;
		return ;
	}
	else {
		check_fragment(str, this->_uri.fragment, index);
	}
	std::cout << std::endl;	
}

void	HTTPRequestParser::check_protocol(std::string const& protocol) {
	if (protocol != "HTTP/1.1") {
		std::cout << "ERROR\nINVALID HTTP protocol" << std::endl;
	}
	else {
		this->_protocol = protocol;
	}
}

void	HTTPRequestParser::check_headers(std::vector<std::string> const& headers) {
	size_t	pos;

	for (size_t i = 0; i < headers.size(); i++) {
		std::pair<std::string, std::string>	key_value_pair;
		
		// std::cout << "headers[i] = " << headers[i] << std::endl;
		pos = headers[i].find(": ");
		if (pos == std::string::npos) {
			std::cout << "ERROR IN CHECK HEADERS" << std::endl;
			break ;
		}
		key_value_pair.first = headers[i].substr(0, pos);
		key_value_pair.second = headers[i].substr(pos + 2, headers[i].size());
		this->_headers.insert(key_value_pair);
	}
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
		it != this->_headers.end(); ++it)
	{
		// std::cout << it->first << " " << it->second << "\n";
		if (it->first == "Transfer-Encoding" && it->second == "chunked") {
			this->_request_body_state = CHUNKED;
		}
	}
}

std::string const&	HTTPRequestParser::get_method(void) const {
	return (this->_method);
}

Uri const&	HTTPRequestParser::get_uri(void) const {
	return (this->_uri);
}

std::string const&	HTTPRequestParser::get_protocol(void) const {
	return (this->_protocol);
}

std::map<std::string, std::string> const&	HTTPRequestParser::get_headers(void) const {
	return (this->_headers);
}

std::string const&	HTTPRequestParser::get_body(void) const {
	return (this->_body);
}

std::string const&	HTTPRequestParser::get_chonky_body(void) const {
	return (this->_chonky_body);
}

RequestBody const&	HTTPRequestParser::get_request_body_state(void) const {
	return (this->_request_body_state);
}

std::ostream&	operator<<(std::ostream& os, HTTPRequestParser const& parser) {
	std::string							method = parser.get_method();
	std::string							protocol = parser.get_protocol();
	Uri									uri = parser.get_uri();
	std::map<std::string, std::string>	headers = parser.get_headers();
	
	if (!method.empty()) {
		os << "METHOD = " << method << std::endl;
	}
	if (!uri.scheme.empty()) {
		os << "URI SCHEME = " << uri.scheme << std::endl;
	}
	if (!uri.authority.user_information.empty()) {
		os << "URI AUTHORITY USER INFORMATION = " << uri.authority.user_information << std::endl;
	}
	if (!uri.authority.host.empty()) {
		os << "URI AUTHORITY HOST = " << uri.authority.host << std::endl;
	}
	if (uri.authority.port != 0) {
		os << "URI AUTHORITY PORT = " << uri.authority.port << std::endl;
	}
	if (!uri.path.full.empty()) {
		os << "URI FULL PATH = " << uri.path.full << std::endl;
	}
	if (!uri.path.without_extension.empty()) {
		os << "URI PATH WITHOUT EXTENSION = " << uri.path.without_extension << std::endl;
	}
	if (!uri.path.extension.empty()) {
		os << "URI PATH EXTENSION = " << uri.path.extension << std::endl;
	}
	if (!uri.query.empty()) {
		os << "URI QUERY = ";
		for (std::map<std::string, std::string>::iterator it = uri.query.begin(); it != uri.query.end(); it++) {
			os << (*it).first << "=" << (*it).second << ",";
		}
		os << std::endl;
	}
	if (!uri.fragment.empty()) {
		os << "URI FRAGMENT = " << uri.fragment << std::endl;
	}
	if (!protocol.empty()) {
		os << "PROTOCOL = " << protocol << std::endl;
	}
	if (!headers.empty()) {
		os << "HEADERS" << std::endl;
		for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++) {
			os << "\t" << (*it).first << " = " << (*it).second << std::endl;
		}
		os << std::endl;
	}
	return (os);
}
