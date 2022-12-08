/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/08 14:59:56 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <map>
# include <string>
# include <vector>

# include "../config_parser/server.hpp"
# include "../http_request/request_data.hpp"
# include "../http_request/request_handler.hpp"
# include "../http_request/uri_parser.hpp"
# include "../http_response/response_data.hpp"
# include "../virtual_server/virtual_server.hpp"

class ResponseHandler;

class Connection {
	public:
		Connection();
		~Connection();
		Connection(Connection const& other);
		Connection&	operator=(Connection const& other);
		void										print_request() const;
		void										select_virtual_server();
		int	const&									get_connection_fd() const;
		ResponseData const&							get_response() const;
		RequestData const&							get_request() const;
		VirtualServer const&						get_virtual_server() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		void										set_connection_fd(int const& connection_fd);
		void										set_response(ResponseData const& response);
		void										set_request(RequestData const& request);
		void										set_virtual_server(VirtualServer const& virtual_server);
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
	private:
		ResponseData								_response;
		RequestData									_request;
		VirtualServer								_virtual_server;
		std::pair<int, std::vector<Server> >		_virtual_servers;
		int											_connection_fd;
};

#endif
