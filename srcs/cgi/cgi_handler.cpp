/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi_handler.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 16:39:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/12 12:25:44 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi_handler.hpp"

void	create_cgi_response(Connection& connection, std::string const& file_location) {
	Cgi						cgi_session(connection, file_location);
	ResponseData			data;
	string					status_line;

	// if (!connection.get_request().request_line.method.compare("GET"))
	status_line = "HTTP1/1 200 OK\r\n";
	data.set_full_response(status_line + cgi_session.get_body());
	connection.set_response(data);
}
