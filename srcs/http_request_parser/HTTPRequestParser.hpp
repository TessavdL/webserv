#ifndef HTTP_REQUEST_PARSER_HPP
# define HTTP_REQUEST_PARSER_HPP

# include <string>
# include <vector>
# include <map>

# include "./HTTPRequestLexer.hpp"

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

typedef struct	Authority {
	std::string	user_information;
	std::string host;
	int			port;
}				Authority;

typedef struct	Uri
{
	std::string	scheme;
	Authority	authority;
	std::string	path;
	std::string file_name;
	std::string	query;
	std::string	fragment;
}				Uri;

class HTTPRequestParser {
	public:
		HTTPRequestParser();
		// HTTPRequestParser(std::string const& request);
		~HTTPRequestParser();
		HTTPRequestParser(HTTPRequestParser const& other);
		HTTPRequestParser&					operator=(HTTPRequestParser const& other);
		Method								check_method(std::string const& str);
		void								check_uri(std::string const& uri);
		void								check_headers(std::vector<std::string> const& headers);
		std::string							get_method(void) const;
		Uri									get_uri(void) const;
		std::map<std::string, std::string>	get_headers(void) const;
		std::string							get_body(void) const;
		std::string							get_chonky_body(void) const;
		RequestBody							get_request_body_state(void) const;
	private:
		std::string							_method;
		Uri									_uri;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		std::string							_chonky_body;
		RequestBody							_request_body_state;
};

#endif
