/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_request_parser.hpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:37:40 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/13 17:13:23 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include <string>
# include <vector>
# include <map>

# include "./http_request_lexer.hpp"
# include "./utility_split.hpp"

# define CLRF "\r\n"
# define DOUBLE_CLRF "\r\n\r\n"

enum Method {
	GET = 1,
	POST = 2,
	DELETE = 3,
	INVALID = -1
};

enum RequestBody {
	NORMAL = 0,
	CHUNKED = 1
};

typedef struct							Authority {
	std::string							user_information;
	std::string 						host;
	int									port;
}										Authority;

typedef struct 							Path
{
	std::string							full;
	std::string							without_extension;
	std::string							extension;
}										Path;

typedef struct							Uri
{
	std::string							scheme;
	Authority							authority;
	Path								path;
	std::string 						file_name;
	std::map<std::string, std::string>	query;
	std::string							fragment;
}										Uri;

class HTTPRequestParser {
	public:
		HTTPRequestParser();
		~HTTPRequestParser();
		HTTPRequestParser(HTTPRequestParser const& other);
		HTTPRequestParser&					operator=(HTTPRequestParser const& other);
		Method								check_method(std::string const& str);
		void								check_uri(std::string const& str);
		void								check_protocol(std::string const& protocol);
		void								check_headers(std::vector<std::string> const& headers);
		std::string const&							get_method(void) const;
		Uri	const&									get_uri(void) const;
		std::string const&							get_protocol(void) const;
		std::map<std::string, std::string> const&	get_headers(void) const;
		std::string const&							get_body(void) const;
		std::string const&							get_chonky_body(void) const;
		RequestBody const&							get_request_body_state(void) const;
	private:
		std::string							_method;
		Uri									_uri;
		std::string							_protocol;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_chonky_body;
		RequestBody							_request_body_state;
};

std::ostream&	operator<<(std::ostream& os, HTTPRequestParser const& parser);

#endif
