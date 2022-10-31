/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   connection.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/31 11:43:36 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/31 15:53:21 by tevan-de      ########   odam.nl         */
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
		Connection();
		~Connection();
		Connection(Connection const& other);
		Connection&	operator=(Connection const& other);
		// int const&									get_connection_fd() const;
		std::pair<int, std::vector<Server> > const&	get_virtual_servers() const;
		HTTPRequestLexer const&						get_request() const;
		Response const&								get_response() const;
		void										set_virtual_servers(std::pair<int, std::vector<Server> > virtual_servers);
	private:
		// int											_connection_fd;
		std::pair<int, std::vector<Server> >		_virtual_servers;
		HTTPRequestLexer							_request;
		Response									_response;
};

#endif
