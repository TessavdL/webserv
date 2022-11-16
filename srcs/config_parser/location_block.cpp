/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/15 19:11:46 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/location_block.hpp"

LocationBlock::LocationBlock(void) {
	return ;
}

LocationBlock::LocationBlock(Lexer::t_locations location) {
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
					throw LexerParserException("Error; autoindex is not \"on\" or \"off\"");
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
			throw LexerParserException("Unknown limit_except argument");
	}
	if (seen_get > 1 || seen_post > 1 || seen_delete > 1 || seen_head > 1)
		throw LexerParserException("Repeated limit_except argument");
}

vector<string> const&			LocationBlock::get_path_and_optional_modifier() const {
	return (this->_path_and_optional_modifier);
}

vector<string> const&	LocationBlock::get_limit_except() const {
	return (this->_limit_except);
}
