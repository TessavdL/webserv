#ifndef PARSER_HTTP_REQUEST_HPP
# define PARSER_HTTP_REQUEST_HPP

# include <string>
# include <vector>

# define CLRF "\r\n"
# define DOUBLE_CLRF "\r\n\r\n"

enum Method {
	GET = 1,
	POST = 2,
	DELETE = 3,
	INVALID = -1
};

class ParserHTTPRequest {
	public:
		ParserHTTPRequest();
		ParserHTTPRequest(std::string const& request);
		~ParserHTTPRequest();
		ParserHTTPRequest(ParserHTTPRequest const& other);
		ParserHTTPRequest&	operator=(ParserHTTPRequest const& other);

	private:

};

#endif
