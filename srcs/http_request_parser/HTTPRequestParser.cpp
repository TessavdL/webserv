#include "ParserHTTPRequest.hpp"

ParserHTTPRequest::ParserHTTPRequest(void) {

}

ParserHTTPRequest::ParserHTTPRequest(std::string const& request) {

}

ParserHTTPRequest::~ParserHTTPRequest(void) {

}

ParserHTTPRequest::ParserHTTPRequest(ParserHTTPRequest const& other) {

}

ParserHTTPRequest&	ParserHTTPRequest::operator=(ParserHTTPRequest const& other) {

}


// static Method	get_method(std::string const& str) {
// 	const char *method_types[3] = {
// 		"GET",
// 		"POST",
// 		"DELETE"
// 	};

// 	for (int i = 0; i < 3; i++) {
// 		if (str == std::string(method_types[i])) {
// 			return (static_cast<Method>(i));
// 		}
// 	}
// 	return (INVALID);
// }

// void	LexerHTTPRequest::check_method(std::string const& str) {
// 	size_t		pos = str.find(" ");
// 	std::string substring;
	
// 	if (pos != std::string::npos) {
// 		substring = str.substr(0, pos);
// 	}
// 	else {
// 		this->_request_line.method = INVALID;
// 	}

// 	this->_request_line.method = get_method(substring);
// }