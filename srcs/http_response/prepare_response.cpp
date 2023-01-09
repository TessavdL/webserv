/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_response.cpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 14:53:47 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 18:24:03 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/prepare_response.hpp"

void	prepare_response(Connection& client) {
	Color::Modifier green(Color::FG_GREEN);
	Color::Modifier def(Color::FG_DEFAULT);
	ResponseHandler	response_handler;

	response_handler.handle_response(client);
	if (response_handler.get_status() == ResponseHandler::CGI) {
		client.set_response_string(client.get_response().get_cgi_response());
	}
	else {
		ResponseGenerator response_string;

		response_string.generate_response_string(client.get_response());
		std::cout << green << response_string << def;
		client.set_response_string(response_string.get_full_response());
	}
	SendData*	send_data = new SendData(client.get_response_string());
	client.set_send_data(send_data);
}
