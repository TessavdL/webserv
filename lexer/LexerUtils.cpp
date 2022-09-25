#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>
#include <vector>
#include "LexerUtils.hpp"

std::string	get_line_until_delimeter(std::string const& str, const char* delimeter) {
	std::string	line;
	size_t		pos;

	pos = str.find_first_of(delimeter);
	if (pos != std::string::npos) {
		line = str.substr(0, pos);
	}
	else {
		line = str;
	}
	return (line);
}

// trim from end of string (right)
std::string right_trim(std::string const& str, const char* to_trim) {
    std::string str_trimmed = str;
	str_trimmed.erase(str_trimmed.find_last_not_of(to_trim) + 1);
    return (str_trimmed);
}

// trim from beginning of string (left)
std::string left_trim(std::string const& str, const char* to_trim) {
	std::string str_trimmed = str;
    str_trimmed.erase(0, str_trimmed.find_first_not_of(to_trim));
    return (str_trimmed);
}

// trim from both ends of string (right then left)
std::string trim(std::string const& str, const char* to_trim) {
    return (left_trim(right_trim(str, to_trim), to_trim));
}

void erase_substring(std::string& str, const std::string& to_erase) {
    size_t pos = str.find(to_erase);
    if (pos != std::string::npos) {
        str.erase(pos, to_erase.length());
    }
}

bool	is_valid_server_start(std::string str) {
	std::string s = trim(str, " \t\n\r\f\v");
	if (s == "server") {
		return (true);
	}
	return (false);
}

size_t	count_server_blocks(std::string str) {
	size_t	server_blocks = 0;
	size_t	pos = str.find("server");
	size_t	pos_server_name;

	if (pos == std::string::npos) {
		throw (LexerParserException());
	}
	while (pos != std::string::npos) {
		pos_server_name = str.find("server_name");
		if (pos_server_name != pos) {
			server_blocks++;
		}
		str = str.substr(pos + sizeof("server"), str.length());
		pos = str.find("server");
	}
	return (server_blocks);
}

static int even_or_uneven(std::string str) {
	int count = 0;

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '{') {
            count++;
        }
        else {
            count--;
        }
	}
	if (count == 0) {
		return (0);
	}
	else {
		return (1);
	}
}

struct is_bracket : std::unary_function<char, bool>
{
    bool operator()(char c) const { return c == '{' || c == '}'; }
};

void	check_brackets(std::string str) {
	size_t	open;
	size_t	close;
	std::string	substr;
	std::string substr2;

	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
	if (str.empty() || str[0] != '{') {
		throw (LexerParserException());
	}

	while (!str.empty() && str.size() > 1) {
		close = str.find_first_of('}');
		substr = str.substr(0, close + 1);
		open = substr.find_last_of('{');
		substr2 = substr.substr(open, substr.size() - open);
		if (even_or_uneven(substr2) == 1) {
			throw (LexerParserException());
		}
		else {
			str.erase(close, 1);
			str.erase(open, 1);
		}
	}
	if (str.empty()) {
		return ;
	}
	else {
		throw (LexerParserException());
	}
}

// bool	is_valid_location_start(std::string str) {
// 	std::string	line;
// 	std::string	line_part;
// 	const char	*spaces = " \t\n\r\f\v";
// 	line = trim(str, spaces);
// 	line_part = get_line_until_delimeter(line, spaces);
// 	if (line_part == "location") {
// 		erase_substring(line, line_part);
// 		line = trim(line, spaces);
// 		line_part = get_line_until_delimeter(line, spaces);
// 		if (line_part == "=") {
// 			line = line.substr(1, line.length());
// 			line = trim(line, spaces);
// 		}
// 		if (!line.empty() && line.find_first_of(spaces) == std::string::npos) {
// 			return (true);
// 		}
// 	}
// 	return (false);
// }

// size_t	find_location_start(std::string const& str) {
// 	size_t		pos = str.find("location");
// 	if (pos != std::string::npos) {
// 		std::string	remainder = str.substr(pos, str.length());
// 		for (size_t i = 0; i < remainder.size(); i++) {

// 		}
// 	}
// 	return (std::string::npos);
// }

// size_t	find_location_end(std::string const& str) {

// }

// std::pair<size_t, size_t>	get_start_and_end_of_server_block(std::string str) {
// 	if (is_valid_server_start(get_line_until_delimeter(str, "{")) == false) {
// 		return (std::pair<size_t, size_t>());
// 	}
// 	size_t	open = str.find("{");
// 	if (open == std::string::npos) {
// 		return (std::pair<size_t, size_t>());
// 	}
// 	size_t	depth = 1;
// 	for (size_t i = 0; i + open + 1< str.size(); i++) {
// 		if (str[i + open + 1] == '{') {
// 			depth++;
// 		}
// 		else if (str[i + open + 1] == '}') {
// 			depth--;
// 		}
// 		if (depth == 0) {
// 			return (std::pair<size_t, size_t>(open, i + 2));
// 		}
// 	}
// 	return (std::pair<size_t, size_t>());
// }

// struct is_bracket : std::unary_function<char, bool>
// {
//     bool operator()(char c) const { return c == '{' || c == '}'; }
// };

// bool	check_brackets(std::string str) {
// 	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
// 	if (str.empty() || str[0] != '{' && str[str.length() -1] != '}') {
// 		return (false);
// 	}
// 	std::string	substring = str.substr(1, str.length() - 2);
// 	for (size_t i = 0; i < substring.size(); i += 2) {
// 		if (!(substring[i] && substring[i + 1] && substring[i] == '{' && substring[i + 1] == '}')) {
// 			return (false);
// 		}
// 	}
// 	return (true);
// }

// std::string	get_input_from_config_file(std::string const& file_name) {
// 	std::ifstream	input_stream;
// 	std::string		input;
	
// 	input_stream.open(file_name);
// 	if (!input_stream) {
// 		std::cout << "error:\n" << "get_input_from_config_file\n" << "failed to open config file" << std::endl;
// 	}
// 	else {
// 		std::ostringstream ss;
// 		ss << input_stream.rdbuf();
// 		input.append(ss.str());
// 	}
// 	return (input);
// }

// int main(void) {
// 	// std::string str("    hi    ");
// 	// std::string str_trimmed = trim(str, " ");
// 	// std::cout << str << std::endl;
// 	// std::cout << str_trimmed << std::endl;
// 	// std::string location_OK("    location = /images   ");
// 	// std::string	location_KO(" location =");
// 	// std::cout << is_valid_location_start(location_OK) << is_valid_location_start(location_KO) << std::endl;
// 	// std::string brackets("{{{}{}{}}}");
// 	// std::string brackets2("{{}{}}{}}");
// 	// std::cout << std::boolalpha << check_brackets(brackets) << std::endl;
// 	// std::cout << std::boolalpha << check_brackets(brackets2) << std::endl;
// 	std::string input;
// 	input = get_input_from_config_file("config_contents.txt");
// 	std::vector<std::string>	server_blocks;
// 	std::string					server_block;
// 	std::pair<size_t, size_t>	start_and_end_of_server_block;
// 	start_and_end_of_server_block = get_start_and_end_of_server_block(input);
// 	server_block = input.substr(start_and_end_of_server_block.first, start_and_end_of_server_block.second);
// 	server_blocks.push_back(server_block);
// 	std::cout << server_blocks[0] << "|" << std::endl;
// 	std::string					substring = input.substr(start_and_end_of_server_block.first + start_and_end_of_server_block.second, input.length());
// 	std::cout << substring << std::endl;
// 	return (0);
// }
