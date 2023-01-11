/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:03:34 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <map>
# include <string>
# include <ctime>
# include <vector>

# include "./colors.hpp"
# include "../config_parser/server.hpp"
# include "../http_request/request_data.hpp"
# include "../http_request/request_handler.hpp"
# include "../http_request/uri_parser.hpp"
# include "../http_response/response_data.hpp"
# include "../virtual_server/virtual_server.hpp"

class Connection {
	public:
		Connection();
		~Connection();
		Connection(Connection const& other);
		Connection&	operator=(Connection const& other);

		void										save_body_and_total_bytes_read();
		void										save_request_line_and_headers();
		void										print_request() const;

		void										reset_response_data();
		void										print_response() const;

		void										handle_rewrite();
		void										select_virtual_server();

		// GETTERS
		int	const&									get_connection_fd() const;
		RequestData const&							get_request() const;
		VirtualServer const&						get_virtual_server() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		time_t const&								get_time() const;

		// SETTERS
		void										set_connection_fd(int const& connection_fd);
		void										set_request(RequestData const& request);
		void										set_virtual_server(VirtualServer const& virtual_server);
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
		void										set_time();

		RequestHandler								request_handler;
		ResponseData								response;
	private:
		int											_connection_fd;
		RequestData									_request;
		std::pair<int, std::vector<Server> >		_virtual_servers;
		VirtualServer								_virtual_server;
		time_t										_time;
};

#endif
