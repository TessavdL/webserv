#ifndef LEXER_UTILS_HPP
# define LEXER_UTILS_HPP

#include "Exception.hpp"

std::string	get_line_until_delimeter(std::string const& str, const char* delimeter);
bool	is_valid_server_start(std::string str);
void erase_substring(std::string& str, const std::string& to_erase);
std::string trim(std::string const& str, const char* to_trim);
std::string left_trim(std::string const& str, const char* to_trim);
std::string right_trim(std::string const& str, const char* to_trim);
size_t	count_server_blocks(std::string str);
void	check_brackets(std::string str);

#endif
