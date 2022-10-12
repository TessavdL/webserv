#ifndef UTILITY_SPLIT_HPP
# define UTILITY_SPLIT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::vector<std::string>			split_string_on_delimeter(std::string const& str, char c);
std::pair<std::string, std::string>	split_string_in_half(std::string const& str, std::string delimiter);
std::pair<std::string, std::string>	split_string_in_half_on_any_match(std::string const& str, std::string delimiter);

#endif
