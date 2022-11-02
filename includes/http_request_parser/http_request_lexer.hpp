/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_request_lexer.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:37:35 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/02 17:06:58 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_LEXER_HPP
# define HTTP_REQUEST_LEXER_HPP

# include <map>
# include <string>
# include <vector>

# include "./uri_parser.hpp"
# include "./utility_split.hpp"

# define CLRF "\r\n"
# define DOUBLE_CLRF "\r\n\r\n"

class HTTPRequestParser;

class HTTPRequestLexer {
	public:
		enum State {
			REQUEST_START = 0,
			REQUEST_LINE = 1,
			REQUEST_HEADERS = 2,
			REQUEST_BODY = 3,
			REQUEST_ERROR = -1
		};

		HTTPRequestLexer();
		~HTTPRequestLexer();
		HTTPRequestLexer(HTTPRequestLexer const& other);
		HTTPRequestLexer&							operator=(HTTPRequestLexer const& other);
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

std::ostream&	operator<<(std::ostream& os, HTTPRequestLexer const& lexer);

#endif
