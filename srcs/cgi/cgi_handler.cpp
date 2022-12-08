/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi_handler.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 16:39:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/08 14:35:00 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi_handler.hpp"

void	create_cgi_response(Connection& connection, std::string const& file_location) {
	Cgi						cgi_session(connection, file_location);
	ResponseData			data;
	map<string, string> 	headers;

	headers["Content-Length"] = to_string(cgi_session.get_body().size());
	headers["Content-Type"] = "text/plain";
	data.set_status_code(200);
	data.set_reason_phrase(get_reason_phrase(200));
	data.set_headers(headers);
	data.set_body(cgi_session.get_body());
	connection.set_response(data);
}
