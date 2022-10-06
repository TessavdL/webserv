/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 14:52:42 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:19:50 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/server_config.hpp"

ServerConfig::ServerConfig(void) {
	return ;
}

ServerConfig::ServerConfig(t_server server) {
	get_directives(server);
	for (vector<t_locations>::iterator it = server.locations.begin(); it != server.locations.end(); ++it) {
		this->_location_blocks.push_back(*it);
	}
}

ServerConfig::ServerConfig(ServerConfig const& other) {
	*this = other;
	return ;
}

ServerConfig	&ServerConfig::operator=(ServerConfig	const& rhs) {
	if (this != &rhs)
	{
		this->_server_name = rhs._server_name;
		this->_listen = rhs._listen;
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

void			ServerConfig::get_directives(t_server server) {
	for (vector<string>::iterator it = server.directives.begin(); it != server.directives.end(); ++it) {
		string	first_word = (*it).substr(0, (*it).find(' '));
		switch (hash_string(first_word))
		{
			case	SERVER_NAME:
				helper_split(this->_server_name, *it);
				break ;
			case	LISTEN:
				helper_split(this->_listen, *it);
				break ;
			case	ROOT:
				helper_split(this->_root, *it);
				break ;
			case	CLIENT_MAX_BODY_SIZE:
				helper_split(this->_client_max_body_size, *it);
				break ;
			case	INDEX:
				helper_split(this->_index, *it);
				break ;
			case	ERROR_PAGE:
				helper_split(this->_error_page, *it);
				break ;
			case	AUTOINDEX:
				helper_split(this->_autoindex, *it);
				break ;
			default:
				cout << *it << " is not a supported directive in the server block." << endl;
				exit (1);
		}
	}
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

int			ServerConfig::helper_split(vector<pair<vector<int>, string>> error_page, string to_split) {
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
	error_page.push_back(pair(tmp_int, tmp[tmp.size() - 1]));
	return (0);
}
