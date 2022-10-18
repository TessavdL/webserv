/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 14:52:42 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/13 16:58:38 by jelvan-d      ########   odam.nl         */
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

ServerConfig	&ServerConfig::operator=(ServerConfig	const& rhs) {
	if (this != &rhs)
	{
		this->_root = rhs._root;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_index = rhs._index;
		this->_error_page = rhs._error_page;
		this->_autoindex = rhs._autoindex;
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
	else
		return (DIRECTIVE_LIST_ERROR);
}

void			ServerConfig::helper_split(string &str, string to_split) {
	vector<string> tmp;

	helper_split(tmp, to_split);
	if (tmp.empty())
		throw LexerParserException(to_split.append(" < invalid input"));
	if (tmp.size() > 1)
		throw LexerParserException(to_split.append(" < invalid input"));
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

void			ServerConfig::helper_split(vector<pair<vector<int>, string> > &error_page, string to_split) {
	vector<string>	tmp;
	vector<int>		tmp_int;

	helper_split(tmp, to_split);
	if (tmp.size() < 2)
		throw LexerParserException("Error page has too few arguments");
	for (size_t i = 0; i < (tmp.size() - 1); ++i) {
		if (tmp[i].find_first_not_of("0123456789") == string::npos) {
			tmp_int.push_back(stoi(tmp[i]));
		}
		else
			throw LexerParserException("Invalid character in error page's error code");
	}
	error_page.push_back(pair<vector<int>, string>(tmp_int, tmp[tmp.size() - 1]));
}

string const&								ServerConfig::get_root() const {
	return (this->_root);
}

string const&								ServerConfig::get_client_max_body_size() const {
	return (this->_client_max_body_size);
}

string const&								ServerConfig::get_autoindex() const {
	return (this->_autoindex);
}

vector<string> const&						ServerConfig::get_index() const {
	return (this->_index);
}

vector<pair<vector<int>, string> > const&	ServerConfig::get_error_page() const {
	return (this->_error_page);
}
