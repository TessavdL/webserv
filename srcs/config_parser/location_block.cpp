/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/05 14:32:18 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/location_block.hpp"

LocationBlock::LocationBlock(void) {
	return ;
}

LocationBlock::LocationBlock(Lexer::t_locations location) {
	this->_client_max_body_size = -1;
	helper_split(this->_path_and_optional_modifier, location.path_and_optional_modifier);
	error_check_path_and_optional_modifier(this->_path_and_optional_modifier);
	get_directives(location);
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

void		LocationBlock::get_directives(Lexer::t_locations location) {
	for (vector<string>::iterator it = location.directives.begin(); it != location.directives.end(); ++it) {
		string	first_word = (*it).substr(0, (*it).find(' '));
		switch (hash_string(first_word))
		{
			case	ROOT:
				helper_split(this->_root, *it);
				break ;
			case	CLIENT_MAX_BODY_SIZE:
				helper_split(this->_client_max_body_size_in_string, *it);
				resolve_client_max_body_size(this->_client_max_body_size, this->_client_max_body_size_in_string);
				break ;
			case	LIMIT_EXCEPT:
				this->_limit_except.clear();
				helper_split(this->_limit_except, *it);
				error_check_limit_except(this->_limit_except);
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
					throw LexerParserException("Invalid autoindex value");
				break ;
			case	CGI:
				helper_split(this->_cgi, *it);
				break ;
			case	RETURN:
				helper_split(this->_return, *it);
				break ;
			case	REWRITE:
				helper_split(this->_rewrite, *it);
				break ;
			default:
				exit (1);
		}
	}
}

void		LocationBlock::error_check_limit_except(vector<string> const& limit_except) const {
	int	seen_get(0);
	int	seen_post(0);
	int	seen_delete(0);
	int	seen_head(0);

	if (limit_except.empty()) {
		throw LexerParserException("Empty limit except directive");
	}
	for (vector<string>::const_iterator it = limit_except.begin(); it != limit_except.end(); ++it) {
		if ((*it) == "GET")
			++seen_get;
		else if ((*it) == "POST")
			++seen_post;
		else if ((*it) == "DELETE")
			++seen_delete;
		else if ((*it) == "HEAD")
			++seen_head;
		else
			throw LexerParserException("Invalid limit except argument");
	}
	if (seen_get > 1 || seen_post > 1 || seen_delete > 1 || seen_head > 1)
		throw LexerParserException("Repeated limit except argument");
}

void	LocationBlock::error_check_path_and_optional_modifier(vector<string> const& path_and_optional_modifier) const {
	if (path_and_optional_modifier.empty()) {
		throw LexerParserException("Empty location path");
	}
	if (path_and_optional_modifier.size() == 2 && path_and_optional_modifier[0].compare("=")) {
		throw LexerParserException("Invalid optional modifier for location");
	}
	if (path_and_optional_modifier.size() > 2) {
		throw LexerParserException("Too many arguments for location path and modifier");
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
