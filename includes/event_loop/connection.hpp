/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 17:33:12 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <map>
# include <string>
# include <vector>

# include "./colors.hpp"
# include "../config_parser/server.hpp"
# include "../http_request/request_data.hpp"
# include "../http_request/request_handler.hpp"
# include "../http_request/uri_parser.hpp"
# include "../http_response/response_data.hpp"
# include "../http_response/send_data.hpp"
# include "../virtual_server/virtual_server.hpp"

class SendData;

class Connection {
	public:
		Connection();
		~Connection();
		Connection(Connection const& other);
		Connection&	operator=(Connection const& other);

		void										print_request() const;
		
		void										handle_rewrite();
		void										select_virtual_server();
		int	const&									get_connection_fd() const;
		std::string const&							get_response_string() const;
		ResponseData const&							get_response() const;
		RequestData const&							get_request() const;
		SendData*									get_send_data() const;
		VirtualServer const&						get_virtual_server() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		void										set_connection_fd(int const& connection_fd);
		void										set_response_string(std::string const& response_string);
		void										set_response(ResponseData const& response);
		void										set_request(RequestData const& request);
		void										set_send_data(SendData* send_data);
		void										set_virtual_server(VirtualServer const& virtual_server);
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
		void										save_request(int bytes_in_data, int listen_backlog_size);
		void										save_request_line_and_headers();
		RequestHandler								request_handler;
				bool const&	get_ready() const;
		void	set_ready_true();
	private:
		int											_connection_fd;
		ResponseData								_response;
		std::string									_response_string;
		RequestData									_request;
		SendData*									_send_data;
		VirtualServer								_virtual_server;
		std::pair<int, std::vector<Server> >		_virtual_servers;
				bool								_ready;
};

#endif
