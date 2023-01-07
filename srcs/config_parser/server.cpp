/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:27:15 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:50:38 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/server.hpp"

Server::Server(void) {
	return ;
}

Server::Server(ConfigLexer::t_server server) {
	this->_root = "html";
	this->_client_max_body_size = 1;
	this->_autoindex = "off";
	this->_server_name.push_back("");
	this->_listen.push_back("80");
	this->_listen.push_back("8000");
	this->_index.push_back("index.html");
	get_directives(server);
	for (vector<ConfigLexer::t_locations>::iterator it = server.locations.begin(); it != server.locations.end(); ++it) {
		this->_location_blocks.push_back(LocationBlock(*it));
	}
}

Server::Server(Server const& other) : ServerConfig(other) {
	*this = other;
	return ;
}

Server	&Server::operator=(Server	const& rhs) {
	if (this != &rhs)
	{
		this->_server_name = rhs._server_name;
		this->_listen = rhs._listen;
		this->_root = rhs._root;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_index = rhs._index;
		this->_error_page = rhs._error_page;
		this->_autoindex = rhs._autoindex;
		this->_location_blocks = rhs._location_blocks;
		this->_host_and_port = rhs._host_and_port;
	}
	return (*this);
}

Server::~Server(void) {
	return ;
}

void			Server::get_directives(ConfigLexer::t_server server) {
	for (vector<string>::iterator it = server.directives.begin(); it != server.directives.end(); ++it) {
		string	first_word = (*it).substr(0, (*it).find(' '));
		switch (hash_string(first_word))
		{
			case	SERVER_NAME:
				helper_split(this->_server_name, *it);
				break ;
			case	LISTEN:
				helper_split(this->_listen, *it);
				error_check_listen(this->_listen);
				break ;
			case	ROOT:
				helper_split(this->_root, *it);
				break ;
			case	CLIENT_MAX_BODY_SIZE:
				helper_split(this->_client_max_body_size_in_string, *it);
				resolve_client_max_body_size(this->_client_max_body_size, this->_client_max_body_size_in_string);
				break ;
			case	INDEX:
				helper_split(this->_index, *it);
				break ;
			case	ERROR_PAGE:
				helper_split(this->_error_page, *it);
				break ;
			case	AUTOINDEX:
				helper_split(this->_autoindex, *it);
				if (this->_autoindex != "on" && this->_autoindex != "off")
					throw ConfigException("Error; autoindex is not \"on\" or \"off\"");
				break ;
			case	RETURN:
				helper_split(this->_return, *it);
				break ;
			case	REWRITE:
				helper_split(this->_rewrite, *it);
				break ;
			default:
				cout << *it << " is not a supported directive in the server block." << endl;
				exit (1);
		}
	}
}

void			Server::error_check_listen(vector<string> const& listen) {
	pair<string, string> hostname_port_split;
	
	for (vector<string>::const_iterator it = listen.begin(); it != listen.end(); ++it) {
		if ((*it).find(":") != string::npos) {
			hostname_port_split = split_string_in_half((*it), ":");
			if (hostname_port_split.first.empty())
				hostname_port_split.first = "localhost";
			if (hostname_port_split.second.empty())
				hostname_port_split.second = "80";
			else if (hostname_port_split.second.find_first_not_of("0123456789") != string::npos)
				throw ConfigException("Invalid character found in listen port");
		}
		else {
			if ((*it).find_first_not_of("0123456789") == string::npos) {
				hostname_port_split.first = "localhost";
				hostname_port_split.second = (*it);
			}
			else {
				hostname_port_split.first = (*it);
				hostname_port_split.second = "80";
			}
		}
		this->_host_and_port.push_back(pair<string, int>(hostname_port_split.first, atoi(hostname_port_split.second.c_str())));
	}
}

vector<string> const&				Server::get_server_name() const {
	return (this->_server_name);
}

vector<string> const&				Server::get_listen() const {
	return (this->_listen);
}

vector<pair<string, int> > const&	Server::get_host_and_port() const {
	return (this->_host_and_port);
}

vector<LocationBlock> const&		Server::get_location_block() const {
	return (this->_location_blocks);
}

std::ostream&	operator<<(std::ostream& os, Server const& server_block) {
	int	location_block_number(1);

	os << "server block:" << endl << endl;
	os << "root = " << server_block.get_root() << endl;
	os << "client max body size = " << server_block.get_client_max_body_size() << endl;
	os << "autoindex = " << server_block.get_autoindex() << endl;
	for (vector<string>::const_iterator it = server_block.get_index().begin(); it != server_block.get_index().end(); ++it)
		os << "index = " << *it << endl;
	for (vector<pair<vector<int>, string> >::const_iterator it = server_block.get_error_page().begin(); it != server_block.get_error_page().end(); ++it) {
		os << "error page = ";
		for (vector<int>::const_iterator it2 = (*it).first.begin(); it2 != (*it).first.end(); ++it2)
			os << (*it2) << " ";
		os << (*it).second << endl;
	}
	for (vector<string>::const_iterator it = server_block.get_server_name().begin(); it != server_block.get_server_name().end(); ++it)
		os << "server name = " << *it << endl;
	for (vector<string>::const_iterator it = server_block.get_listen().begin(); it != server_block.get_listen().end(); ++it)
		os << "listen = " << *it << endl;
	os << endl;
	for (vector<LocationBlock>::const_iterator it = server_block.get_location_block().begin(); it != server_block.get_location_block().end(); ++it) {
		os << "location block[" << location_block_number << "]" << endl;
		os << "\troot = " << (*it).get_root() << endl;
		os << "\tclient max body size = " << (*it).get_client_max_body_size() << endl;
		os << "\tautoindex = " << (*it).get_autoindex() << endl;
		for (vector<string>::const_iterator it2 = (*it).get_index().begin(); it2 != (*it).get_index().end(); ++it2)
			os << "\tindex = " << (*it2) << endl;
		for (vector<pair<vector<int>, string> >::const_iterator it2 = (*it).get_error_page().begin(); it2 != (*it).get_error_page().end(); ++it2) {
			os << "\terror page = ";
			for (vector<int>::const_iterator it3 = (*it2).first.begin(); it3 != (*it2).first.end(); ++it3)
				os << (*it3) << " ";
			os << (*it2).second << endl;
		}
		for (vector<string>::const_iterator it2 = (*it).get_path_and_optional_modifier().begin(); it2 != (*it).get_path_and_optional_modifier().end(); ++it2)
			os << "\tpath and optional modifier = " << (*it2) << endl;
		for (vector<string>::const_iterator it2 = (*it).get_limit_except().begin(); it2 != (*it).get_limit_except().end(); ++it2)
			os << "\tlimit except = " << (*it2) << endl;
		location_block_number++;
	}
	return (os);
}
