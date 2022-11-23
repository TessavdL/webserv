/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 13:55:34 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <map>
#include <string>
#include <vector>

#include "../config_parser/server.hpp"
#include "../http_request/request_handler.hpp"
#include "../http_request/uri_parser.hpp"
#include "../http_response/response_data.hpp"
#include "../virtual_server/virtual_server.hpp"

class ResponseHandler;

class Connection {
	public:
		typedef struct							s_request_line {
			std::string							method;
			Uri									uri;
			std::string							protocol;
		}										t_request_line;
		typedef struct							s_request {
			t_request_line						request_line;
			std::map<std::string, std::string>	headers;
			std::string							body;
			int									bytes_in_data;
			int									total_bytes_read;
		}										t_request;
		Connection();
		~Connection();
		Connection(Connection const& other);
		Connection&	operator=(Connection const& other);
		void										print_request() const;
		void										select_virtual_server();
		int	const&									get_connection_fd() const;
		ResponseData const&							get_response() const;
		Connection::t_request const&				get_request() const;
		VirtualServer const&						get_virtual_server() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		// size_t const&								get_location_index(void) const;
		// size_t const&								get_server_index(void) const;
		void										set_connection_fd(int const& connection_fd);
		void										set_response(ResponseData const& response);
		void										set_request(Connection::t_request const& request);
		void										set_virtual_server(VirtualServer const& virtual_server);
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
		// void										set_location_index(size_t server_index);
		// void										set_server_index(size_t server_index);
	private:
		ResponseData								_response;
		Connection::t_request						_request;
		// size_t										_location_index;
		// size_t										_server_index;
		VirtualServer								_virtual_server;
		std::pair<int, std::vector<Server> >		_virtual_servers;
		int											_connection_fd;
};

#endif
