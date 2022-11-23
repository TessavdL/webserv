/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_handler.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:44:56 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 13:50:05 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HANDLER_HPP
# define REQUEST_HANDLER_HPP

# include <iostream>
# include <map>
# include <sstream>
# include <string>
# include <vector>

# include "./uri_parser.hpp"
# include "./utility_split.hpp"

# define CLRF "\r\n"
# define DOUBLE_CLRF "\r\n\r\n"

class RequestHandler {
	public:
		enum State {
			REQUEST_START = 0,
			REQUEST_LINE = 1,
			REQUEST_HEADERS = 2,
			REQUEST_BODY = 3,
			REQUEST_ERROR = -1
		};

		RequestHandler();
		~RequestHandler();
		RequestHandler(RequestHandler const& other);
		RequestHandler&								operator=(RequestHandler const& other);
		void										process_request(std::string const& request);
		void										full_request_line(std::string const& str, size_t& index);
		void										tokenize_request_line(std::string const& str);
		void										full_headers(std::string const& str, size_t& index);
		void										tokenize_request_headers(std::string const& str);
		void										create_headers_map(std::vector<std::string> const& v);
		void										process_request_start(std::string const& request);
		void										go_method(std::string const& str, size_t& index);
		void										go_uri(std::string const& str, size_t& index);
		void										go_protocol(std::string const& str, size_t& index);
		void										go_headers(std::string const& str, size_t& index);
		void										go_body(std::string const& str, size_t& index);
		void										go_chonky_body(std::string const& str, size_t &index);
		State const&								get_state(void) const;
		std::string const&							get_remainder(void) const;
		std::string const&							get_request_line_method(void) const;
		std::string const&							get_request_line_uri_raw(void) const;
		std::string const&							get_request_line_protocol(void) const;
		Uri const&									get_request_line_uri(void) const;
		std::map<std::string, std::string> const&	get_headers(void) const;
		std::string const&							get_body(void) const;
	private:
		std::string									_remainder;
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