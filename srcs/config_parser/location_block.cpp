/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 17:30:40 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/location.hpp"

Location::Location(void) {
	return ;
}

Location::Location(Lexer::t_locations locations) : _path_and_optional_modifier(locations.path_and_optional_modifier) {
	get_directives(locations);
}

Location::Location(Location const& other) {
	*this = other;
	return ;
}

Location &Location::operator=(Location const& rhs) {
	if (this != &rhs)
	{
		this->_path_and_optional_modifier = rhs._path_and_optional_modifier;
	}	
	return (*this);
}

Location::~Location(void) {
	return ;
}

void		Location::get_directives(Lexer::t_locations locations) {
	for (vector<string>::iterator it = locations.directives.begin(); it != locations.directives.end(); ++it) {
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
				break ;
			default:
				cout << *it << " is not a supported directive in the location block." << endl;
				exit (1);
		}
	}
}
