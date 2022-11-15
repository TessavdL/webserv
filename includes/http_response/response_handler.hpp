/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:45:33 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 18:33:01 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONES_HANDLER_HPP
# define RESPONES_HANDLER_HPP

#include "../event_loop/connection.hpp"
#include "../http_request_parser/utility_path.hpp"
#include "./error_checking.hpp"

#include <iostream>

class ResponseHandler {
	public:
		ResponseHandler() {}
		ResponseHandler(Connection& client);
		~ResponseHandler();
		ResponseHandler(ResponseHandler const& other);
		ResponseHandler&	operator=(ResponseHandler const& other);

		bool								client_or_server_error_occured(void) const;
		void								create_error_response(Connection& client);
		void								handle_response(Connection& client);
		std::map<std::string, std::string>	create_headers(size_t body_size);
		void								create_get_response(Connection& client, std::string const& file_location);
		void								handle_get_request(Connection& client, Connection::t_request request);
	private:
		int	_status_code;
		
};

#endif
