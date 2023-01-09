/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi_handler.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 16:39:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/09 18:26:01 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi_handler.hpp"

void	create_cgi_response(Connection& connection, std::string const& file_location) {
	Cgi						cgi_session(connection, file_location);
	ResponseData			data;
	string					status_line;

	if (!connection.get_request().get_method().compare("GET"))
		status_line = "HTTP/1.1 200 OK\r\n";
	else if (!connection.get_request().get_method().compare("POST"))
		status_line = "HTTP/1.1 201 Created\r\n";
	data.set_cgi_response(status_line + cgi_session.get_body());
	connection.set_response(data);
}
