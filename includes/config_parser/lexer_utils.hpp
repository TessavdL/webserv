#ifndef LEXER_UTILS_HPP
# define LEXER_UTILS_HPP

# define EVEN 0
# define UNEVEN 1
# define WHITESPACE " \t\n\r\f\v"

# include "exception.hpp"

#include <string>

bool		is_valid_server_start(std::string str);
bool		is_valid_location_start(std::string str);
void 		erase_substring(std::string& str, const std::string& to_erase);
std::string trim(std::string const& str, const char* to_trim);
std::string left_trim(std::string const& str, const char* to_trim);
std::string right_trim(std::string const& str, const char* to_trim);
size_t		count_blocks(std::string const& str);
void		check_brackets(std::string str);

#endif
