/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 14:52:42 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/06 16:20:55 by jelvan-d      ########   odam.nl         */
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

int			ServerConfig::helper_split(string &str, string to_split) {
	vector<string> tmp;

	helper_split(tmp, to_split);
	if (tmp.empty())
		return (1);
	if (tmp.size() > 1)
		return (1);
	str = tmp[0];
	return (0);
}

int			ServerConfig::helper_split(vector<string> &str, string to_split) {
	stringstream ss(to_split);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> tmp(begin, end);
	str = tmp;
	if (!str.empty())
		str.erase(str.begin());
	return (0);
}

int			ServerConfig::helper_split(vector<pair<vector<int>, string> > error_page, string to_split) {
	vector<string>	tmp;
	vector<int>		tmp_int;

	helper_split(tmp, to_split);
	if (tmp.size() < 2)
		return (1);
	for (size_t i = 0; i < (tmp.size() - 1); ++i) {
		if (tmp[i].find_first_not_of("0123456789") == string::npos)
			tmp_int.push_back(stoi(tmp[i]));
		else
			return (1);
	}
	error_page.push_back(pair<vector<int>, string>(tmp_int, tmp[tmp.size() - 1]));
	return (0);
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
