/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   receive_request.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 17:53:11 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 15:11:33 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/receive_request.hpp"

static int prepare_error_response_to_client(Connection& client, int const status_code) {
	ResponseData	response_data;

	response_data.set_status_code(status_code);
	client.set_response(response_data);
	return (-1);
}

static bool	is_ready_to_check_request_line_and_headers(RequestHandler::State state) {
	if (state == RequestHandler::REQUEST_CHECK) {
		return (true);
	}
	return (false);
}

int	parse_request(Connection& client, std::string const& input) {
	try {
		client.request_handler.process_request(input);
	}
	catch (RequestException const& e) {
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
			if (e2.get_status_code() == 100) {
				client.request_handler.set_state(RequestHandler::REQUEST_BODY);
				parse_request(client, "");
			}
			return (prepare_error_response_to_client(client, e2.get_status_code()));
		}
	}
	return (0);
}

// void	receive_request(Connection& client, int connection_fd, int listen_backlog_size) {
// 	int		bytes_read(1);
// 	int		total_bytes_read(0);
// 	char	buf[BUFF_SIZE + 1];

// 	std::cout << "listen_backlog_lize = " << listen_backlog_size << std::endl;
// 	while (bytes_read > 0) {
// 		bytes_read = recv(connection_fd, buf, BUFF_SIZE, 0);
// 		std::cout << "bytes read = " << bytes_read << std::endl;
// 		buf[bytes_read] = '\0';
// 		total_bytes_read += bytes_read;
// 		if (parse_request(client, std::string(buf, bytes_read))) {
// 			return ;
// 		}
// 	}
// 	client.save_request(total_bytes_read, listen_backlog_size);
// }

void	receive_request(Connection& client, int connection_fd, int listen_backlog_size) {
	int		bytes_read(1);
	char	buf[BUFF_SIZE + 1];
	size_t	total_bytes_read = client.get_request().get_total_bytes_read();

	std::cout << "listen_backlog_lize = " << listen_backlog_size << std::endl;
	bytes_read = recv(connection_fd, buf, BUFF_SIZE, 0);
	std::cout << "bytes read = " << bytes_read << std::endl;
	buf[bytes_read] = '\0';
	if (parse_request(client, std::string(buf, bytes_read))) {
		return ;
	}
	// maybe remove read event from kqueue in case of an error 
	if (bytes_read < BUFF_SIZE || bytes_read == 0)
		client.save_request(total_bytes_read, listen_backlog_size);
}
