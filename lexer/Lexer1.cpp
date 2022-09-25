#include "Lexer1.hpp"

Lexer::Lexer(void) {}

Lexer::~Lexer(void) {}

Lexer::Lexer(Lexer const& other) : server_blocks(other.server_blocks) {}

Lexer&	Lexer::operator=(Lexer const& other) {
	if (this != &other) {
		this->server_blocks = other.server_blocks;
	}
	return (*this);
}

std::string	Lexer::get_input_from_config_file(std::string const& file_name) const {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open(file_name);
	if (!input_stream) { // open failed
		throw (LexerParserException());
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		input.append(ss.str());
	}
	input_stream.close();
	return (input);
}

static int	even_or_uneven(std::string const& str) {
	int count = 0;

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '{') {
            count++;
        }
        else { // str[i] == '}'
            count--;
        }
	}
	if (count == 0) {
		return (0);
	}
	else { // count != 0{
		return (1);
	}
}

struct is_bracket : std::unary_function<char, bool>
{
    bool operator()(char c) const { return c == '{' || c == '}'; }
};

std::pair<size_t, size_t>	Lexer::get_start_and_end_of_server_block(std::string str) {
	if (is_valid_server_start(get_line_until_delimeter(str, "{")) == false) {
		throw (LexerParserException());
	}
	size_t	open = str.find("{");
	if (open == std::string::npos) {
		throw (LexerParserException());
	}
	size_t	depth = 1;
	for (size_t i = 0; i + open + 1< str.size(); i++) {
		if (str[i + open + 1] == '{') {
			depth++;
		}
		else if (str[i + open + 1] == '}') {
			depth--;
		}
		if (depth == 0) {
			return (std::pair<size_t, size_t>(open, i + 2));
		}
	}
	throw (LexerParserException());
}

std::vector<std::string>	Lexer::get_server_blocks(std::string const& input) {
	std::string					str = input;
	std::string					server_block;
	std::vector<std::string>	server_blocks;
	std::pair<size_t, size_t>	start_end;
	size_t						n_server_blocks;

	n_server_blocks = count_server_blocks(str);
	for (size_t i = 0; i < n_server_blocks; i++) {
		start_end = get_start_and_end_of_server_block(str);
		server_block = str.substr(start_end.first, start_end.second);
		server_blocks.push_back(server_block);
		erase_substring(str, str.substr(0, start_end.first + start_end.second));
	}
	return (server_blocks);
}
