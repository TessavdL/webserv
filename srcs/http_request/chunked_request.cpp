/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chunked_request.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/07 12:23:47 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/21 17:40:09 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/chunked_request.hpp"

static int hexadecimal_to_decimal(std::string hex) {
	unsigned int 		x;   
	std::stringstream	ss;

	ss << std::hex << hex;
	ss >> x;
	return (static_cast<int>(x));
}

static bool	is_hexadecimal(std::string const& s) {
	for (size_t i = 0; i < s.length(); i++) {
		if (!isxdigit(s[i])) {
			return (false);
		}
	}
	return (true);
}

static int	handle_hexadecimal(std::string str, size_t& index) {
	str = str.substr(index);

	std::string	hexadecimal;
	size_t 		pos = str.find(CRLF);

	if (pos != std::string::npos) {
		hexadecimal = str.substr(0, pos);
		if (!is_hexadecimal(hexadecimal)) {
			return (-1);
		}
		index += (pos + 2);
		return (hexadecimal_to_decimal(hexadecimal));
	}
	return (-1);
}

static std::string	handle_pair(std::string str, size_t& index) {
	size_t	start = index;

	int number_of_bytes_in_data = handle_hexadecimal(str, index);
	if (number_of_bytes_in_data == -1) {
		throw (RequestException(400, "handle_pair"));
	}

	int total = 0;
	std::string res;

	str = str.substr(index);
	for (size_t pos = str.find(CRLF); pos != std::string::npos; pos = str.find(CRLF)) {
		std::string to_add = str.substr(0, pos);
		if (to_add.empty() && number_of_bytes_in_data == 0) {
			return ("");
		}
		res += to_add;
		index += to_add.length() + 2;
		str = str.substr(index);
		total += to_add.length();
		if (total == number_of_bytes_in_data) {
			return (res);
		}
		if (total > number_of_bytes_in_data) {
			throw (RequestException(400, "handle_pair"));
		}
		res.append("\r\n");
		total += 2;
	}
	index = start;
	return ("");
}

std::string chunked_request(std::string const& str, std::string& remainder) {
	size_t		index = 0;
	std::string	res;

	while (1) {
		std::string hex_data_pair;
		hex_data_pair = handle_pair(str, index);
		std::cout << "|" << hex_data_pair << "|" << std::endl;
		if (hex_data_pair.empty())
			break ;
		res.append(hex_data_pair);
	}
	remainder.clear();
	remainder = str.substr(index);
	return (res);
}
