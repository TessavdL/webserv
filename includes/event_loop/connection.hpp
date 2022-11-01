/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/01 17:14:15 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTION_HPP
# define CONNECTION_HPP

#include <map>
#include <string>
#include <vector>

#include "../../includes/config_parser/server.hpp"
#include "../../includes/http_request_parser/http_request_lexer.hpp"
#include "../../includes/http_response/response.hpp"

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
		// int const&									get_connection_fd() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		Connection::t_request const&				get_request() const;
		Response const&								get_response() const;
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
		void										set_request(Connection::t_request const& request);
	private:
		// int											_connection_fd;
		std::pair<int, std::vector<Server> >		_virtual_servers;
		Connection::t_request						_request;
		Response									_response;
};

#endif
