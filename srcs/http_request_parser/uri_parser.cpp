/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   uri_parser.cpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/02 14:18:39 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/02 16:49:39 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/uri_parser.hpp"

Uri::Uri() : _authority_port(0) {

}

Uri::~Uri() {

}

Uri::Uri(Uri const& other) {
    *this = other;
}

Uri&  Uri::operator=(Uri const& other) {
    if (this != &other) {
        this->_scheme = other._scheme;
        this->_authority_user_information = other._authority_user_information;
        this->_authority_host = other._authority_host;
        this->_authority_port = other._authority_port;
        this->_path_full = other._path_full;
        this->_path_extension = other._path_extension;
        this->_path_without_extension = other._path_without_extension;
        this->_query = other._query;
    }
    return (*this);
}

void    Uri::parse_uri(std::string const& str) {
    size_t		index = 0;
	
	parse_scheme(str, index);
	parse_authority(str, index);
	parse_path(str, index);
	parse_query(str, index);
}

void    Uri::parse_scheme(std::string const& str, size_t& index) {
	this->_scheme = string_until_deilimeter(str.substr(index), "://");
    if (!this->_scheme.empty()) {
        std::transform(this->_scheme.begin(), this->_scheme.end(), this->_scheme.begin(), ::tolower);
        index += this->_scheme.length() + 3;
    }
}

static std::string full_authority_string(std::string const& str, size_t& index) {
    size_t       pos = str.substr(index).find("/");
    std::string ret;

    if (pos != std::string::npos) {
        ret = str.substr(index, pos);
    }
    return (ret);
}

void    Uri::parse_authority(std::string const& str, size_t& index) {
    std::string authority_string = full_authority_string(str, index);

    if (authority_string.empty())
        return ;

    size_t pos_user_information_end;
    size_t pos_port_start;

    index += authority_string.length();
    pos_user_information_end = authority_string.find("@");
    if (pos_user_information_end != std::string::npos) {
        this->_authority_user_information = authority_string.substr(0, pos_user_information_end);
        authority_string.erase(0, pos_user_information_end);
    }
    pos_port_start = str.substr(index).find(":");
    if (pos_port_start != std::string::npos) {
        std::string port_string = authority_string.substr(pos_port_start + 1);
        authority_string.erase(pos_port_start);
        if (!port_string.empty()) {
            this->_authority_port = stoi(port_string);
        }
    }
    if (!authority_string.empty()) {
        this->_authority_host = authority_string;
    }
}

void    Uri::parse_path(std::string const& str, size_t& index) {
    size_t  pos = str.substr(index).find("?");
    size_t  pos_extension_start;
    
    this->_path_full = str.substr(index, pos);
    index += this->_path_full.length();
    pos_extension_start = this->_path_full.find(".");
    if (pos != std::string::npos) {
        std::pair<std::string, std::string> p = split_string_in_half(this->_path_full, ".");
        this->_path_without_extension = p.first;
        this->_path_extension = p.second;
    }
}

void    Uri::parse_query(std::string const& str, size_t index) {
    if (str.substr(index).empty() || (str[index] && str[index] != '?')) {
        return ;
    }
    std::vector<std::string>	query_seperated = split_string_on_delimeter(str.substr(index + 1), '&');
    for (size_t i = 0; i < query_seperated.size(); i++) {
        if (!query_seperated[i].empty()) {
            std::pair<std::string, std::string> p = split_string_in_half(query_seperated[i], "=");
            this->_query.insert(p);
        }
    }
}

std::string const&  Uri::get_scheme(void) const {
    return (this->_scheme);
}

std::string const&  Uri::get_authority_user_information(void) const {
    return (this->_authority_user_information);
}

std::string const&  Uri::get_authority_host(void) const {
    return (this->_authority_host);
}

int const&  Uri::get_authority_port(void) const {
    return (this->_authority_port);
}

std::string const&  Uri::get_path_full(void) const {
    return (this->_path_full);
}

std::string const&  Uri::get_path_extension(void) const {
    return (this->_path_extension);
}

std::string const&  Uri::get_path_without_extension(void) const {
    return (this->_path_without_extension);
}

std::map<std::string, std::string> const&  Uri::get_query(void) const {
    return (this->_query);
}

std::ostream&   operator<<(std::ostream& os, Uri const& uri) {
    std::string                         scheme = uri.get_scheme();
    std::string                         authority_user_information = uri.get_authority_user_information();
    std::string                         authority_host = uri.get_authority_host();
    int                                 authority_port = uri.get_authority_port();
    std::string                         path_full = uri.get_path_full();
    std::string                         path_extension = uri.get_path_extension();
    std::string                         path_without_extension = uri.get_path_without_extension();
    std::map<std::string, std::string>  query = uri.get_query();

    if (!scheme.empty()) {
        os << scheme << std::endl;
    }
    if (!authority_user_information.empty()) {
        os << authority_user_information << std::endl;
    }
    if (!authority_host.empty()) {
        os << authority_host << std::endl;
    }
    os << authority_port << std::endl;
    if (!path_full.empty()) {
        os << path_full << std::endl;
    }
    if (!path_extension.empty()) {
        os << path_extension << std::endl;
    }
    if (!path_without_extension.empty()) {
        os << path_without_extension << std::endl;
    }
    if (!query.empty()) {
        os << "query string" << std::endl;
        for (std::map<std::string, std::string>::const_iterator it = query.begin(); it != query.end(); it++) {
            os << "\t" << it->first << "=" << it->second << std::endl;
        }
    }
    return (os);
}
