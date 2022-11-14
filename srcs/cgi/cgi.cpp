/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:28 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/01 18:08:47 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi.hpp"

Cgi::Cgi(void) {
	return ;
}

Cgi::Cgi(Cgi const& other) {
	*this = other;
}

Cgi&	Cgi::operator=(Cgi const& rhs) {
	if (this != &rhs)
		return (*this);
	return (*this);
}

Cgi::~Cgi(void) {
	return ;
}
