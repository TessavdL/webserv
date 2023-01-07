/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/07 22:24:53 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/location_block.hpp"

LocationBlock::LocationBlock(void) {
	return ;
}

LocationBlock::LocationBlock(ConfigLexer::t_locations location) {
	this->_client_max_body_size = -1;
	this->_return.first = -1;
	helper_split(this->_path_and_optional_modifier, location.path_and_optional_modifier);
	error_check_path_and_optional_modifier(this->_path_and_optional_modifier);
	initialise_seen_directives();
	get_directives(location);
	check_duplicate_directives();
}

LocationBlock::LocationBlock(LocationBlock const& other) : ServerConfig(other) {
	*this = other;
	return ;
}

LocationBlock &LocationBlock::operator=(LocationBlock const& rhs) {
	if (this != &rhs)
	{
		this->_autoindex = rhs._autoindex;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_cgi = rhs._cgi;
		this->_error_page = rhs._error_page;
		this->_index = rhs._index;
		this->_path_and_optional_modifier = rhs._path_and_optional_modifier;
		this->_return = rhs._return;
		this->_rewrite = rhs._rewrite;
		this->_root = rhs._root;
		this->_limit_except = rhs._limit_except;
	}	
	return (*this);
}

LocationBlock::~LocationBlock(void) {
	return ;
}

void		LocationBlock::get_directives(ConfigLexer::t_locations location) {
	for (vector<string>::iterator it = location.directives.begin(); it != location.directives.end(); ++it) {
		string	first_word = (*it).substr(0, (*it).find(' '));
		switch (hash_string(first_word))
		{
			case	ROOT:
				this->_seen_root++;
				helper_split(this->_root, *it);
				break ;
			case	CLIENT_MAX_BODY_SIZE:
				this->_seen_client_max_body_size++;
				helper_split(this->_client_max_body_size_in_string, *it);
				resolve_client_max_body_size(this->_client_max_body_size, this->_client_max_body_size_in_string);
				break ;
			case	LIMIT_EXCEPT:
				this->_seen_limit_except++;
				helper_split(this->_limit_except, *it);
				error_check_limit_except(this->_limit_except);
				break ;
			case	INDEX:
				this->_seen_index++;
				helper_split(this->_index, *it);
				if (this->_index.empty()) {
					throw ConfigException("Empty index directive");
				}
				break ;
			case	ERROR_PAGE:
				helper_split(this->_error_page, *it);
				break ;
			case	AUTOINDEX:
				this->_seen_autoindex++;
				helper_split(this->_autoindex, *it);
				if (this->_autoindex != "on" && this->_autoindex != "off")
					throw ConfigException("Invalid autoindex value");
				break ;
			case	CGI:
				this->_seen_cgi++;
				helper_split(this->_cgi, *it);
				break ;
			case	RETURN:
				this->_seen_return++;
				helper_split(this->_return, *it);
				break ;
			case	REWRITE:
				this->_seen_rewrite++;
				helper_split(this->_rewrite, *it);
				break ;
			default:
				throw ConfigException("Invalid directive in location block");
		}
	}
}

void		LocationBlock::error_check_limit_except(vector<string> const& limit_except) const {
	int	seen_get(0);
	int	seen_post(0);
	int	seen_delete(0);

	if (limit_except.empty()) {
		throw ConfigException("Empty limit except directive");
	}
	for (vector<string>::const_iterator it = limit_except.begin(); it != limit_except.end(); ++it) {
		if ((*it) == "GET")
			++seen_get;
		else if ((*it) == "POST")
			++seen_post;
		else if ((*it) == "DELETE")
			++seen_delete;
		else
			throw ConfigException("Invalid limit except argument");
	}
	if (seen_get > 1 || seen_post > 1 || seen_delete > 1)
		throw ConfigException("Repeated limit except argument");
}

void	LocationBlock::error_check_path_and_optional_modifier(vector<string> const& path_and_optional_modifier) const {
	if (path_and_optional_modifier.empty()) {
		throw ConfigException("Empty location path");
	}
	if (path_and_optional_modifier.size() == 2 && path_and_optional_modifier[0].compare("=")) {
		throw ConfigException("Invalid optional modifier for location");
	}
	if (path_and_optional_modifier.size() > 2) {
		throw ConfigException("Too many arguments for location path and modifier");
	}
}

void			LocationBlock::initialise_seen_directives(void) {
	this->_seen_root = 0;
	this->_seen_client_max_body_size = 0;
	this->_seen_limit_except = 0;
	this->_seen_index = 0;
	this->_seen_autoindex = 0;
	this->_seen_cgi = 0;
	this->_seen_return = 0;
	this->_seen_rewrite = 0;
}

void			LocationBlock::check_duplicate_directives(void) {
	if (this->_seen_root > 1 ||
		this->_seen_client_max_body_size > 1 ||
		this->_seen_limit_except > 1 ||
		this->_seen_index > 1 ||
		this->_seen_autoindex > 1 ||
		this->_seen_cgi > 1 ||
		this->_seen_return > 1 ||
		this->_seen_rewrite > 1) {
			throw ConfigException("Duplicate directive in location block");
		}
}

pair<string, string> const&	LocationBlock::get_cgi(void) const {
	return (this->_cgi);
}

vector<string> const&	LocationBlock::get_limit_except(void) const {
	return (this->_limit_except);
}

vector<string> const&	LocationBlock::get_path_and_optional_modifier(void) const {
	return (this->_path_and_optional_modifier);
}
