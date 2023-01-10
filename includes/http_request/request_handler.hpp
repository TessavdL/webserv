/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:44:56 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 15:42:58 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>

# include "./chunked_request.hpp"
# include "./uri_parser.hpp"
# include "./utility_split.hpp"
# include "./request_exception.hpp"

# define CRLF "\r\n"
# define CRLFLEN 4

# define DOUBLE_CRLF "\r\n\r\n"
# define MAX_HEADER_SIZE 1000
# define MAX_URI_SIZE 2000

class RequestHandler {
	public:
		enum State {
			REQUEST_START = 0,
			REQUEST_LINE = 1,
			REQUEST_HEADERS = 2,
			REQUEST_CHECK = 3,
			REQUEST_BODY = 4
		};

		RequestHandler();
		~RequestHandler();
		RequestHandler(RequestHandler const& other);
		RequestHandler&								operator=(RequestHandler const& other);
		void										process_request(std::string const& request);
		void										process_request_start(std::string const& request);

		void										handle_request_line(std::string const& str, size_t& index);
		void										full_request_line(std::string const& str, size_t& index);
		void										tokenize_request_line(std::string const& str);

		void										handle_headers(std::string const& str, size_t& index);
		void										full_headers(std::string const& str, size_t& index);
		void										tokenize_request_headers(std::string const& str);
		void										create_headers_map(std::vector<std::string> const& v);

		void										handle_body(std::string const& str, size_t& index);
		void										handle_normal_body(std::string const& str, size_t& index);
		void										handle_chunked_body(std::string const& str, size_t &index);

		State const&								get_state() const;
		std::string const&							get_remainder() const;
		std::string const&							get_request_line_method() const;
		std::string const&							get_request_line_uri_raw() const;
		std::string const&							get_request_line_protocol() const;
		Uri const&									get_request_line_uri() const;
		std::map<std::string, std::string> const&	get_headers() const;
		std::string const&							get_body() const;
		int const&									get_total_bytes_read() const;
		
		void										set_state(RequestHandler::State state);
	private:
		std::string									_remainder;
		int											_total_bytes_read;
		std::string									_request_line_full;
		std::string									_request_line_method;
		std::string									_request_line_uri_raw;
		Uri											_request_line_uri;
		std::string									_request_line_protocol;
		std::vector<std::string>					_request_headers_tokens;
		std::string									_request_headers_full;
		std::map<std::string, std::string>			_request_headers;
		std::string									_request_body;
		State										_state;
};

std::ostream&	operator<<(std::ostream& os, RequestHandler const& lexer);

#endif
