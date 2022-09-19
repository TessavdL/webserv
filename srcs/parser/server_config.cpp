/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 14:52:42 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/19 19:13:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser/server_config.hpp"

server_config::server_config(void) {
	return ;
}

server_config::server_config(t_server server) : _autoindex(false) {
	for (vector<string>::iterator it = server.directives.begin(); it != server.directives.end(); ++it) {
		switch (this->hash_string(*it))
		{
			case	SERVER_NAME:
				
				break ;
			case	LISTEN:
				break ;
			case	ROOT:
				break ;
			case	CLIENT_MAX_BODY_SIZE:
				break ;
			case	LIMIT_EXCEPT:
				break ;
			case	INDEX:
				break ;
			case	ERROR_PAGE:
				break ;
			case	AUTOINDEX:
				
				break ;
			default:
				cout << *it << " is not a supported directive" << endl;
				exit (1);
		}
	}
}

server_config::server_config(server_config const& other) {
	*this = other;
	return ;
}

server_config	&server_config::operator=(server_config	const& rhs) {
	if (this != &rhs)
	{
		this->_server_name = rhs._server_name;
		this->_listen = rhs._listen;
		this->_root = rhs._root;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_limit_except = rhs._limit_except;
		this->_index = rhs._index;
		this->_error_page = rhs._error_page;
		this->_autoindex = rhs._autoindex;
	}
	return (*this);
}

server_config::~server_config(void) {
	return ;
}

directives_list	server_config::hash_string(string const& directive) {
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

void		server_config::helper_split(string &str, string to_split) {
	char		*cstr = new char[to_split.length() + 1];
	strcpy(cstr, to_split.c_str());
	char			*p = std::strtok(cstr, " ");
	string			tmp(p);
	str = tmp;
}
