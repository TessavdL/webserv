/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:45:33 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/22 18:23:33 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONES_HANDLER_HPP
# define RESPONES_HANDLER_HPP

#include "../event_loop/connection.hpp"
#include "../http_request_parser/utility_path.hpp"
#include "../virtual_server/search_for_file.hpp"
#include "./error_checking.hpp"
#include "./response_utils.hpp"
#include "./error_page_string.hpp"

#include <iostream>

class ResponseHandler {
	public:

		typedef enum	response_state {
			DEFAULT_ERROR = -1,
			CUSTOM_ERROR = -2,
			DIRECTORY_LIST = 1,
			UNSET = 0,
			GET = 2,
			POST = 3,
			DELETE = 4,
			CGI = 5
		}	state;

		ResponseHandler();
		~ResponseHandler();
		ResponseHandler(ResponseHandler const& other);
		ResponseHandler&	operator=(ResponseHandler const& other);

		void								create_directory_list_response(Connection& client, std::string const& page);
		void								create_error_response(Connection& client, std::string const& file_location, std::string const& file_content);
		std::map<std::string, std::string>	create_headers(Connection& client, std::string const& file_name, size_t body_size);
		void								create_get_response(Connection& client, std::string const& file_location, std::string file_content);
		std::string							get_file_content(std::string const& file_location);
		std::string							error_page_location_handler(std::pair<std::string, bool> error_page);
		std::string							handle_error_page(VirtualServer const& virtual_server);
		std::string							file_location_handler(VirtualServer const& virtual_server, std::pair<std::string, bool> const& file_location);
		void								handle_response(Connection& client);
		void								handle_get_response(Connection& client, Connection::t_request const& request);

	private:
		int		_status_code;
		state	_state;
		
};

#endif
