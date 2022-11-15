/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:45:33 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 14:22:55 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONES_HANDLER_HPP
# define RESPONES_HANDLER_HPP

#include "../event_loop/connection.hpp"
#include "./error_checking.hpp"

class ResponseHandler {
	public:
		ResponseHandler(Connection& client);
		~ResponseHandler();
		ResponseHandler(ResponseHandler const& other);
		ResponseHandler&	operator=(ResponseHandler const& other);
		bool	client_or_server_error_occured(void) const;
		void	create_error_response(Connection& client);
		void	handle_response(Connection& client);
		void	create_get_response(Connection& client, std::string const& file_location);
		void	handle_get_request(Connection& client, Connection::t_request request);
	private:
		int	_status_code;
		ResponseHandler() {}
		
};

#endif
