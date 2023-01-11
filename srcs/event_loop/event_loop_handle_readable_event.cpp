/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop_handle_readable_event.cpp               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 16:24:24 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 16:59:35 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/event_loop_actions.hpp"

static int prepare_error_response_to_client(Connection& client, int const status_code) {
	ResponseData	response_data;

	response_data.set_status_code(status_code);
	client.response = response_data;
	return (-1);
}

static bool	is_ready_to_check_request_line_and_headers(RequestHandler::State state) {
	if (state == RequestHandler::REQUEST_CHECK) {
		return (true);
	}
	return (false);
}

static int	parse_request(Connection& client, std::string const& input) {
	try {
		client.request_handler.process_request(input);
	}
	catch (RequestException const& e) {
		std::cout << "A bad request was sent\n" << e.what() << "\n";
		std::cout << "status_code = " << e.get_status_code() << "\n";
		return (prepare_error_response_to_client(client, e.get_status_code()));
	}
	if (is_ready_to_check_request_line_and_headers(client.request_handler.get_state())) {
		try {
			client.save_request_line_and_headers();
			client.select_virtual_server();
			check_request_line_and_headers(client, client.get_request());
			client.request_handler.set_state(RequestHandler::REQUEST_BODY);
			parse_request(client, "");
		}
		catch (RequestException const& e2) {
			if (e2.get_status_code() != 100) {
				std::cout << "A bad request was sent\n" << e2.what() << "\n";
				std::cout << "status_code = " << e2.get_status_code() << "\n";
			}
			if (e2.get_status_code() == 100) {
				client.request_handler.set_state(RequestHandler::REQUEST_BODY);
				parse_request(client, "");
			}
			return (prepare_error_response_to_client(client, e2.get_status_code()));
		}
	}
	return (0);
}

static int	receive_request(Connection& client, int connection_fd, int listen_backlog_size) {
	int		bytes_read(1);
	char	buf[BUFF_SIZE + 1];
	int		size = BUFF_SIZE;

	if (listen_backlog_size < BUFF_SIZE) {
		size = listen_backlog_size;
	}
	bytes_read = recv(connection_fd, buf, size, 0);
	buf[bytes_read] = '\0';
	if (parse_request(client, std::string(buf, bytes_read))) {
		return (-2);
	}
	return (bytes_read);
}

static void	handle_request(Connection& client, int kq, struct kevent& event) {
	int	listen_backlog_size = event.data;
	int	bytes_read = receive_request(client, event.ident, listen_backlog_size);

	if (bytes_read == listen_backlog_size) {
		client.save_body_and_total_bytes_read();
		client.print_request();
		add_write_event_to_kqueue(kq, event.ident);
	}
	else if (bytes_read == -2) {
		delete_read_event_from_kqueue(kq, &event, event.ident);
		add_write_event_to_kqueue(kq, event.ident);
	}
}

void	handle_readable_event(std::map<int, Connection>& connections, int const kq, struct kevent& event) {
	std::cout << "readable event [" << event.ident << "]\n\n";
	if (is_client(connections, event.ident)) {
		Connection& client = connections[event.ident];
		handle_request(client, kq, event);
	}
}
