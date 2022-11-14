/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:28 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/14 16:49:37 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi.hpp"

Cgi::Cgi(void) {
	return ;
}

Cgi::Cgi(Connection const&	connection) {
	create_env(connection, connection.get_request());
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

void	Cgi::create_env(Connection const& connection, Connection::t_request const& request) {
	if (request.request_line.method.compare("POST"))
		this->_env["CONTENT_LENGTH"] = request.bytes_in_data;
	else
		this->_env["CONTENT_LENGTH"] = to_string(request.request_line.uri.get_query_string().size());
}
