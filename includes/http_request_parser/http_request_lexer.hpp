/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_request_lexer.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:37:35 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/13 16:23:57 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_LEXER_HPP
# define HTTP_REQUEST_LEXER_HPP

# include <string>
# include <vector>

# include "./http_request_parser.hpp"

# define CLRF "\r\n"
# define DOUBLE_CLRF "\r\n\r\n"

class HTTPRequestParser;

class HTTPRequestLexer {
	public:
		typedef struct	RequestLine
		{
			std::string	method;
			std::string	uri;
			std::string	protocol;
		}				RequestLine;

		enum State {
			REQUEST_START = 0,
			REQUEST_LINE_METHOD = 1,
			REQUEST_LINE_URI = 2,
			REQUEST_LINE_PROTOCOL = 3,
			REQUEST_HEADERS = 4,
			REQUEST_BODY = 5,
			REQUEST_ERROR = -1
		};

		HTTPRequestLexer();
		~HTTPRequestLexer();
		HTTPRequestLexer(HTTPRequestLexer const& other);
		HTTPRequestLexer&				operator=(HTTPRequestLexer const& other);
		void							process_request(std::string const& request);
		void							process_request_start(std::string const& request);
		void							go_method(std::string const& str, size_t& index);
		void							go_uri(std::string const& str, size_t& index);
		void							go_protocol(std::string const& str, size_t& index);
		void							go_headers(std::string const& str, size_t& index);
		void							go_body(std::string const& str, size_t& index);
		void							go_chonky_body(std::string const& str, size_t &index);
		State							get_state(void) const;
		std::string						get_remainder(void) const;
		HTTPRequestLexer::RequestLine	get_request_line(void) const;
		std::vector<std::string>		get_headers(void) const;
		std::string						get_body(void) const;
		HTTPRequestParser*				parser;
	private:
		std::string						_remainder;
		RequestLine 					_request_line;
		std::vector<std::string>		_request_headers;
		std::string						_request_body;
		State							_state;
};

std::ostream&	operator<<(std::ostream& os, HTTPRequestLexer const& lexer);

#endif
