/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_handler.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 15:45:33 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/07 20:57:56 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

#include "./error_checking.hpp"
#include "./response_utils.hpp"
#include "./error_page_string.hpp"
#include "../cgi/cgi_handler.hpp"
#include "../event_loop/connection.hpp"
#include "../http_request/utility_path.hpp"
#include "../virtual_server/search_for_file.hpp"

#include <iostream>

class ResponseHandler {
	public:

		enum response_state {
			DEFAULT_ERROR = -1,
			CUSTOM_ERROR = -2,
			DIRECTORY_LIST = 1,
			UNSET = 0,
			GET = 2,
			POST = 3,
			DELETE = 4,
			CGI = 5,
			RETURN = 6,
			CONTINUE = 7
		};

		ResponseHandler();
		~ResponseHandler();
		ResponseHandler(ResponseHandler const& other);
		ResponseHandler&	operator=(ResponseHandler const& other);

		void								handle_response(Connection& client);
		void								handle_delete_response(Connection& client, RequestData const& request_data);
		void								handle_error_response(Connection& client);
		void								handle_get_response(Connection& client, RequestData const& request_data);
		void								handle_post_response(Connection& client, RequestData const& request_data);
		
		void								create_continue_response(Connection& client);
		void								create_delete_response(Connection& client, std::string const& file);
		void								create_directory_list_response(Connection& client, std::string const& page);
		void								create_error_response(Connection& client, std::string const& file_location, std::string const& file_content);
		void								create_get_response(Connection& client, std::string const& file_location, std::string file_content);
		void								create_return_response(Connection& client, std::pair<int, std::string> return_information);
		
		std::map<std::string, std::string>	create_continue_headers();
		std::map<std::string, std::string>	create_headers(Connection& client, std::string const& file_name, size_t body_size);
		
		bool								is_continue(int const& status_code, response_state response_state);

		std::string							error_page_location_handler(std::pair<std::string, bool> error_page);
		std::string							handle_error_page(VirtualServer const& virtual_server);
		std::string							file_location_handler(VirtualServer const& virtual_server, std::pair<std::string, bool> const& file_location);
		std::string const					file_information(Connection& client, RequestData const& request_data);
	
		response_state const&				get_status() const;
		int const&							get_status_code() const;

	private:
		int				_status_code;
		response_state	_state;
		
};

#endif
