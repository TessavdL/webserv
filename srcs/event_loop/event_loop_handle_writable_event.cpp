/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop_handle_writable_event.cpp               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 16:30:15 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/12 16:13:07 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/event_loop_actions.hpp"

static void	send_response_to_client(std::map<int, Connection>& connections, int const kq, struct kevent& event) {
	Connection&	client = connections[event.ident];
	std::string	response_string = client.response.get_repsonse_string();
	const char	*buf = response_string.c_str();
	ssize_t		size = response_string.size();
	ssize_t		bytes_sent = 0;
	ssize_t		total_bytes_sent = client.response.get_total_bytes_sent();

	bytes_sent = send(event.ident, buf + total_bytes_sent, size - total_bytes_sent, 0);
	if (bytes_sent == -1) {
		return ;
	}
	client.response.set_total_bytes_sent(total_bytes_sent + bytes_sent);
	if ((bytes_sent == 0 || client.response.get_total_bytes_sent() == size) && !connection_is_continue(client.response.get_headers())) {
		close(event.ident);
		connections.erase(event.ident);
		std::cout << "closed event identifier [" << event.ident << "]\n\n";
	}
	else if ((bytes_sent == 0 || client.response.get_total_bytes_sent() == size) && connection_is_continue(client.response.get_headers())) {
		delete_write_event_from_kqueue(kq, &event, event.ident);
		add_read_event_to_kqueue(kq, client.get_connection_fd());
		client.reset_response_data();
	}
}

static void	prepare_response(Connection& client) {
	ResponseHandler	response_handler;

	response_handler.handle_response(client);
	if (response_handler.get_status() == ResponseHandler::CGI) {
		client.response.set_response_string(client.response.get_cgi_response());
	}
	else {
		ResponseGenerator response_string;

		response_string.generate_response_string(client.response);
		client.response.set_response_string(response_string.get_full_response());
		client.print_response();
	}
}

void	handle_writable_event(std::map<int, Connection>& connections, int const kq, struct kevent& event) {
	std::cout << "writable event [" << event.ident << "]\n\n";
	if (is_client(connections, event.ident)) {
		Connection& client = connections[event.ident];
		if (!response_is_generated(client.response.get_generated())) {
			prepare_response(client);
			client.response.set_generated(true);
		}
		send_response_to_client(connections, kq, event);
	}
}
