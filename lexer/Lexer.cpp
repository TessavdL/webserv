#include "Lexer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>

Lexer::Lexer(void) {}

Lexer::~Lexer(void) {
	this->_server_blocks.clear();
}

Lexer::Lexer(Lexer const& other) : _server_blocks(other._server_blocks) {}

Lexer&	Lexer::operator=(Lexer const& other) {
	if (this != &other) {
		this->_server_blocks = other._server_blocks;
	}
	return (*this);
}

std::string	Lexer::get_input_from_config_file(std::string const& file_name) const {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open(file_name);
	if (!input_stream) {
		std::cout << "error:\n" << "get_input_from_config_file\n" << "failed to open config file" << std::endl;
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		input.append(ss.str());
	}
	return (input);
}

size_t	Lexer::count_server_blocks(std::string str) const {
	size_t	server_blocks = 0;
	size_t	pos = str.find("server");
	size_t	pos_server_name;

	if (pos == std::string::npos) {
		std::cout << "error:\n" << "count_server_blocks\n" << "no server blocks were found" << std::endl;
		return (0);
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

int	Lexer::even_or_uneven(std::string str) {
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

void	Lexer::check_brackets(std::string str) {
	size_t	open;
	size_t	close;
	std::string	substr;
	std::string substr2;

	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
	if (str.empty() || str[0] != '{') {
		std::cout << "error:\n" << "in check_brackets\n" << "brackets invalid" << std::endl;
		return ;
	}

	while (!str.empty() && str.size() > 1)
	{
		close = str.find_first_of('}');
		substr = str.substr(0, close + 1);
		open = substr.find_last_of('{');
		substr2 = substr.substr(open, substr.size() - open);
		if (even_or_uneven(substr2) == 1) 	
		{
			std::cout << "error:\n" << "in check_brackets\n" << "brackets invalid" << std::endl;
			return ;
		}
		else
		{
			str.erase(close, 1);
			str.erase(open, 1);
		}
	}
	if (str.empty()) {
		std::cout << "brackets OK" << std::endl;
	}
	else {
		std::cout << "error:\n" << "in check_brackets\n" << "brackets KO" << std::endl;
	}
}

void	Lexer::add_directives(std::string str, std::vector<std::string> block) {
	std::string	directive;
	std::string	substring;
	size_t		semicolumn;
	
	for (size_t i = 0; i < str.size(); i++) {
		i += skip_whitespaces(str, i);
		substring = str.substr(i, str.length());
		if (substring.empty()) {
			return ;
		}
		semicolumn = substring.find_first_of(';');
		if (semicolumn == std::string::npos) {
			std::cout << "error:\n" << "in add_directives\n" << "directive not closed with a semicolumn" << std::endl;
			std::cout << str << std::endl;
			return ;
		}
		directive = substring.substr(0, semicolumn);
		// std::cout << directive << std::endl;
		block.push_back(directive);
		i += semicolumn;
	}
}

static void	get_path_and_modifier(Lexer::t_locations& loc, std::string str) {
	size_t pos = str.find("location");
	std::string substring = str.substr(pos, str.length());
	std::string subsubstring = substring.substr(0, substring.find_first_of('{'));
	std::cout << subsubstring << std::endl;
	loc.path_and_optional_modifier = subsubstring;
}

std::string	Lexer::get_block(std::string str, t_server& server_block) {
	size_t		start_block = str.find_first_of('{');
	size_t		end_block = str.find_first_of('}');
	std::string	substring = str.substr(start_block, end_block);
	size_t		start_block_check = substring.find_last_of('{');
	size_t		pos;

	if (start_block_check == 0) {
		substring = substring.substr(1, substring.length() - 2);
		std::string subsubstring = substring.substr(0, substring.find_first_of('}'));
		add_directives(subsubstring, server_block.directives);
		pos = str.find(subsubstring);
		if (pos != std::string::npos) {
			str.erase(pos, subsubstring.length());
			std::cout << str << std::endl;
		}
		pos = str.find_first_of("}");
		if (pos != std::string::npos) {
			str.erase(0, pos + 1);
			std::cout << str << std::endl;
		}
	}
	else {
		std::string location = substring.substr(start_block_check + 1, substring.find_first_of('}') - start_block_check - 1);
		t_locations	loc;
		add_directives(location, loc.directives);
		server_block.locations.push_back(loc);
		pos = str.find(location);
		get_path_and_modifier(loc, str);
		if (pos != std::string::npos) {
			str.erase(pos, location.length());
		}
		pos = str.find(loc.path_and_optional_modifier);
		if (pos != std::string::npos) {
			str.erase(pos, loc.path_and_optional_modifier.length());
		}
		pos = str.find("{}");
		if (pos != std::string::npos) {
			str.erase(pos, 2);
		}
		return (get_block(str, server_block));
	}
	return (str);
}

