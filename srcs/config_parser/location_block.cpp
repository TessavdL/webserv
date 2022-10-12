/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 14:54:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/location_block.hpp"

LocationBlock::LocationBlock(void) {
	return ;
}

LocationBlock::LocationBlock(Lexer::t_locations location) {
	this->_root = "html";
	this->_client_max_body_size = "1m";
	this->_autoindex = "off";
	this->_index.push_back("index.html");
	this->_limit_except.push_back("GET");
	this->_limit_except.push_back("POST");
	this->_limit_except.push_back("DELETE");
	helper_split(this->_path_and_optional_modifier, location.path_and_optional_modifier);
	get_directives(location);
}

LocationBlock::LocationBlock(LocationBlock const& other) {
	*this = other;
	return ;
}

LocationBlock &LocationBlock::operator=(LocationBlock const& rhs) {
	if (this != &rhs)
	{
		this->_path_and_optional_modifier = rhs._path_and_optional_modifier;
		this->_root = rhs._root;
		this->_client_max_body_size = rhs._client_max_body_size;
		this->_index = rhs._index;
		this->_error_page = rhs._error_page;
		this->_autoindex = rhs._autoindex;
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
				helper_split(this->_client_max_body_size, *it);
				break ;
			case	LIMIT_EXCEPT:
				this->_limit_except.clear();
				helper_split(this->_limit_except, *it);
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
					throw LexerParserException("Error; autoindex is not \"on\" or \"off\"");
				break ;
			default:
				exit (1);
		}
	}
}

vector<string> const&			LocationBlock::get_path_and_optional_modifier() const {
	return (this->_path_and_optional_modifier);
}

vector<string> const&	LocationBlock::get_limit_except() const {
	return (this->_limit_except);
}
