/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/19 15:11:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/19 17:23:05 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parser/location.hpp"

location::location(void) {
	return ;
}

location::location(location const& other) : server_config(other) {
	return ;
}

location &location::operator=(location const& rhs) {
	if (this != &rhs)
		server_config::operator=(rhs);
	return (*this);
}

location::~location(void) {
	return ;
}
