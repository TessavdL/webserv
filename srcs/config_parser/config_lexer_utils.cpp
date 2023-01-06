/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_lexer_utils.cpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/06 16:41:53 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:49:01 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/config_lexer_utils.hpp"

std::string	right_trim(std::string const& str, const char* to_trim) {
    std::string str_trimmed = str;
	str_trimmed.erase(str_trimmed.find_last_not_of(to_trim) + 1);
    return (str_trimmed);
}

std::string	left_trim(std::string const& str, const char* to_trim) {
	std::string str_trimmed = str;
    str_trimmed.erase(0, str_trimmed.find_first_not_of(to_trim));
    return (str_trimmed);
}

std::string	trim(std::string const& str, const char* to_trim) {
    return (left_trim(right_trim(str, to_trim), to_trim));
}

void	erase_substring(std::string& str, const std::string& to_erase) {
    size_t pos = str.find(to_erase);
    if (pos != std::string::npos) {
        str.erase(pos, to_erase.length());
    }
}

bool	is_valid_server_start(std::string str) {
	std::string s = trim(str, WHITESPACE);
	if (s == "server") {
		return (true);
	}
	return (false);
}

bool	is_valid_location_start(std::string str) {
	std::string s = trim(str, WHITESPACE);
	if (!s.compare(0, 8, "location")) {
		return (true);
	}
	return (false);
}

size_t	count_blocks(std::string const& str) {
	size_t	depth = 0;
	size_t	n_of_blocks = 0;

	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '{') {
			depth++;
		}
		else if (str[i] == '}') {
			if (depth == 1) {
				n_of_blocks++;
			}
			depth--;
		}
	}
	return (n_of_blocks);
}

static int	even_or_uneven(std::string str) {
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
		return (EVEN);
	}
	else {
		return (UNEVEN);
	}
}

struct is_bracket : std::unary_function<char, bool>
{
    bool operator()(char c) const {
		return (c == '{' || c == '}');
	}
};

struct is_semicolumn : std::unary_function<char, bool>
{
    bool operator()(char c) const {
		return (c == ';');
	}
};

bool check_valid_semicolumn(std::string const& str) {
	if (std::count_if(str.begin(), str.end(), is_semicolumn()) == 1) {
		return (true);
	}
	return (false);
}

void	check_brackets(std::string str) {
	size_t		open;
	size_t		close;
	std::string	substr;
	std::string	substr2;

	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
	if (str.empty() || str[0] != '{') {
		throw (ConfigException("No brackets or brackets don't start with opening brackets"));
	}
	while (!str.empty() && str.size() > 1) {
		close = str.find_first_of('}');
		substr = str.substr(0, close + 1);
		open = substr.find_last_of('{');
		substr2 = substr.substr(open, substr.size() - open);
		if (even_or_uneven(substr2) == UNEVEN) {
			throw (ConfigException("Brackets do not open and close properly"));
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
		throw (ConfigException("Brackets do not open and close properly"));
	}
}

void	check_for_nested_location_blocks(std::string str) {
	int	depth = 0;
	
	str.erase(std::remove_if(str.begin(), str.end(), std::not1(is_bracket())), std::end(str));
	if (str.empty() || str[0] != '{') {
		throw (ConfigException("No brackets or brackets don't start with opening brackets"));
	}
	for (size_t i = 0; i < str.size(); i++) {
		if (str[i] == '{') {
			depth++;
		}
		else {
			depth--;
		}
		if (depth > 2) {
			throw (ConfigException("Nested location blocks are not allowed"));
		}
	}
}
