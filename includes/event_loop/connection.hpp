/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 16:57:09 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <map>
#include <string>
#include <vector>

#include "../../includes/config_parser/server.hpp"
#include "../../includes/http_request_parser/http_request_lexer.hpp"
#include "../../includes/http_request_parser/uri_parser.hpp"
#include "../../includes/http_response/response.hpp"
#include "../includes/virtual_server/select_virtual_server.hpp"
#include "../includes/virtual_server/select_location.hpp"

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
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		Connection::t_request const&				get_request() const;
		ResponseData const&							get_response() const;
		size_t const&								get_location_index(void) const;
		size_t const&								get_server_index(void) const;
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
		void										set_request(Connection::t_request const& request);
		void										set_location_index(size_t server_index);
		void										set_server_index(size_t server_index);
	private:
		std::pair<int, std::vector<Server> >		_virtual_servers;
		Connection::t_request						_request;
		ResponseData								_response;
		size_t										_location_index;
		size_t										_server_index;
};

#endif
