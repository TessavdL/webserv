/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 14:52:42 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/07 18:24:01 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/server_config.hpp"

ServerConfig::ServerConfig(void) {
	return ;
}

ServerConfig::ServerConfig(ServerConfig const& other) {
	*this = other;
	return ;
}

ServerConfig	&ServerConfig::operator=(ServerConfig const& rhs) {
	if (this != &rhs)
	{
		this->_autoindex = rhs._autoindex;
		this->_client_max_body_size_in_string = rhs._client_max_body_size_in_string;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_error_page = rhs._error_page;
		this->_index = rhs._index;
		this->_return = rhs._return;
		this->_rewrite = rhs._rewrite;
		this->_root = rhs._root;
	}
	return (*this);
}

ServerConfig::~ServerConfig(void) {
	return ;
}

directives_list	ServerConfig::hash_string(string const& directive) {
	if (directive == "server_name")
		return (SERVER_NAME);
	if (directive == "listen")
		return (LISTEN);
	if (directive == "root")
		return (ROOT);
	if (directive == "client_max_body_size")
		return (CLIENT_MAX_BODY_SIZE);
	if (directive == "limit_except")
		return (LIMIT_EXCEPT);
	if (directive == "index")
		return (INDEX);
	if (directive == "error_page")
		return (ERROR_PAGE);
	if (directive == "autoindex")
		return (AUTOINDEX);
	if (directive == "cgi")
		return (CGI);
	if (directive == "return")
		return (RETURN);
	if (directive == "rewrite")
		return (REWRITE);
	else
		return (DIRECTIVE_LIST_ERROR);
}

void			ServerConfig::helper_split(string &str, string to_split) {
	vector<string> tmp;

	helper_split(tmp, to_split);
	if (tmp.empty())
		throw ConfigException(to_split.append(" < empty directive"));
	if (tmp.size() > 1)
		throw ConfigException(to_split.append(" < expected 1 argument"));
	str = tmp[0];
}

void			ServerConfig::helper_split(vector<string> &str, string to_split) {
	stringstream ss(to_split);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> tmp(begin, end);
	str = tmp;
	if (!str.empty())
		str.erase(str.begin());
}

void			ServerConfig::helper_split(pair<string, string> &ret, string to_split) {
	vector<string> tmp;

	helper_split(tmp, to_split);
	if (tmp.size() != 2)
		throw ConfigException("CGI has an incorrect amount of arguments");
	ret.first = tmp[0];
	ret.second = tmp[1];
	if (ret.first.compare("php"))
		throw ConfigException("CGI only handles php file extensions, please change to php");
	if (ret.second.compare("/Users/jelvan-d/.brew/Cellar/php/8.1.13/bin/php-cgi"))
		throw ConfigException("Wrong CGI script selected, please select the PHP-CGI script located in '/Users/jelvan-d/.brew/Cellar/php/8.1.13/bin/php-cgi'");
}

void			ServerConfig::helper_split(pair<int, string> &ret, string to_split) {
	vector<string> tmp;

	helper_split(tmp, to_split);
	if (tmp.size() != 1 && tmp.size() != 2)
		throw ConfigException("Return has an incorrect amount of arguments");
	if (tmp.size() == 1) {
		if (tmp[0].find_first_not_of("0123456789") != string::npos)
			throw ConfigException("Invalid character in return status code");
		ret.first = atoi(tmp[0].c_str());
		if (ret.first > 600) {
			throw ConfigException("Invalid status code in return");
		}
		return ;
	}
	ret.second = tmp[1];
}

void			ServerConfig::helper_split(vector<pair<vector<int>, string> > &error_page, string to_split) {
	vector<string>	tmp;
	vector<int>		tmp_int;

	helper_split(tmp, to_split);
	if (tmp.size() < 2)
		throw ConfigException("Error page has too few arguments");
	for (size_t i = 0; i < (tmp.size() - 1); ++i) {
		if (tmp[i].find_first_not_of("0123456789") == string::npos) {
			tmp_int.push_back(atoi(tmp[i].c_str()));
		}
		else
			throw ConfigException("Invalid character in error page's error code");
	}
	error_page.push_back(pair<vector<int>, string>(tmp_int, tmp[tmp.size() - 1]));
}

void			ServerConfig::resolve_client_max_body_size(int& client_max_body_size, string client_max_body_size_in_string) {
	size_t	tmp_pos = client_max_body_size_in_string.find_first_not_of("0123456789");

	if (client_max_body_size_in_string.size() > 4) {
		throw ConfigException("Client max body size too large, please add a value between 0 and 999");
	}
	else if (tmp_pos == string::npos) {
		throw ConfigException("No data type specifier in client max body size");
	}
	else if (client_max_body_size_in_string[tmp_pos] != 'M' && client_max_body_size_in_string[tmp_pos] != 'm') {
		throw ConfigException("Invalid data specifier in client max body size");
	}
	else if (strcmp(client_max_body_size_in_string.c_str() + tmp_pos, "M\0") && strcmp(client_max_body_size_in_string.c_str() + tmp_pos, "m\0")) {
		throw ConfigException("Too many characters in data specifier for client max body size");
	}
	client_max_body_size = atoi(client_max_body_size_in_string.c_str());
}

string const&								ServerConfig::get_autoindex() const {
	return (this->_autoindex);
}

int const&									ServerConfig::get_client_max_body_size() const {
	return (this->_client_max_body_size);
}

vector<pair<vector<int>, string> > const&	ServerConfig::get_error_page() const {
	return (this->_error_page);
}

vector<string> const&						ServerConfig::get_index() const {
	return (this->_index);
}

pair<int, string> const&					ServerConfig::get_return() const {
	return (this->_return);
}

string const&								ServerConfig::get_rewrite() const {
	return (this->_rewrite);
}

string const&								ServerConfig::get_root() const {
	return (this->_root);
}
