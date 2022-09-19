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

std::string	Lexer::get_raw_data(std::string const& file_name) {
	std::ifstream	input_stream;
	std::string		raw_data;
	
	input_stream.open(file_name);
	if (!input_stream) {
		std::cout << "error" << std::endl;
		return (raw_data);
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		raw_data.append(ss.str());
	}
	return (raw_data);
}

size_t	Lexer::count_server_blocks(std::string str) {
	size_t	server_blocks = 0;
	size_t	pos = str.find("server");
	size_t	pos_server_name;

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

struct is_bracket : std::unary_function<char, bool>
{
    bool operator()(char c) const { return c == '{' || c == '}'; }
};

int	Lexer::even_or_uneven(std::string str) {
	int count = 0;
	
	std::cout << "in even or uneven = " << str << std::endl;
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '{')
        {
            count++;
        }
        else if (str[i] == '}')
        {
            count--;
        }
	}
	std::cout << count << std::endl;
	if (count == 0)
	{
		return (0);
	}
	else {
		return (1);
	}
}

void	Lexer::get_server_block(std::string str) {
	size_t	open;
	size_t	close;
	std::string	substr;
	std::string substr2;

	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
	if (str.empty() || str[0] != '{')
	{
		std::cout << "error" << std::endl;
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
			std::cout << "error" << std::endl;
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
		std::cout << "brackets KO" << std::endl;
	}
}

bool	Lexer::check_brackets(std::string const& raw_data) {
	t_server	server_block;
	std::string	str = raw_data;

	str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
	
	size_t		start_block = str.find_first_of("{");
	std::string	substring = str.substr(0, start_block);

	substring.erase(std::remove_if(substring.begin(), substring.end(), ::isspace), substring.end());
	if (substring != "server") {
		std::cout << "error" << std::endl;
		return (false);
	}
	std::string	remainder = str.substr(start_block + 1, str.length()); 
	std::cout << "remainder" << std::endl << remainder << std::endl;
	
	size_t location = remainder.find("location");
	std::cout << location << std::endl;
	std::string	directives = remainder.substr(0, location);
	if (location == std::string::npos)
	{
		std::cout << "no location block" << std::endl;
	}
	add_directives(directives, server_block);

	return (true);
}

void	Lexer::add_directives(std::string str, t_server server_block) {
	std::string directive;
	std::string	substring;
	size_t		semicol;
	size_t		i = 0;

	while (str[i]) {
		while (str[i] && isspace(str[i])) {
			i++;
		}
		substring = str.substr(i, str.length());
		semicol = substring.find_first_of(";");
		directive = substring.substr(0, semicol);
		std::cout << directive << std::endl;
		server_block.directives.push_back(directive);
		i = i + semicol;
		i++;
	}
}
