/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chunked_request.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/07 12:23:47 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/03 16:35:07 by tevan-de      ########   odam.nl         */
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
		std::cout << " crlf yes " << std::endl;
	}
	else {
		std::cout << " crlf no " << std::endl;
	}

	if (pos != std::string::npos) {
		std::cout << "HI ====== " << str << std::endl;
		hexadecimal = str.substr(0, pos);
		if (!is_hexadecimal(hexadecimal)) {
			return (-1);
		}
		index += (pos + 2);
		return (hexadecimal_to_decimal(hexadecimal));
	}
	return (-2);
}

static std::string	handle_pair(std::string const s, size_t& index) {
	size_t	start = index;
	std::string new_string = s;

	int number_of_bytes_in_data = handle_hexadecimal(new_string, index);
	std::cout << "number of bytes in data = " << number_of_bytes_in_data << std::endl;
	// if (str.find(CRLF) == std::string::npos) {

	// }
	if (number_of_bytes_in_data == -1) {
		throw (RequestException(400, "handle_pair"));
	}

	int total = 0;
	std::string res;

	// std::cout << "index = " << index << std::endl;
	std::string str = new_string.substr(index);
	// str = str.substr(index);
	if (str.find(CRLF) == std::string::npos) {
		std::cout << "there is no CRLF" << std::endl;
	}
	for (size_t pos = str.find(CRLF); pos != std::string::npos; pos = str.find(CRLF)) {
		size_t i = 0;
		std::string to_add = str.substr(0, pos);
		
		// std::cout << "pos = " << pos << std::endl;
		// std::cout << "to add =\t|" << to_add.substr(0, 55) << "|" << std::endl;
		std::cout << "is to_add empty?" << std::boolalpha << to_add.empty() << std::endl;
		if (to_add.empty() && number_of_bytes_in_data == 0) {
			return ("");
		}
		res += to_add;
		// std::cout << "res =\t\t|" << res.substr(0, 55) << "|" << std::endl;
		// std::cout << "str =\t\t|" << str.substr(0, 55) << "|" << std::endl;
		// std::cout << "str len\t\t= |" << str.size() << "|" << std::endl;
		// std::cout << "to_add len\t= |" << to_add.size() << "|" << std::endl;
		index += (to_add.size() + 2);
		i += (to_add.size() + 2);
		// std::cout << "index\t\t= |" << index << "|" << std::endl;
		// std::cout << "i = " << i << std::endl;
		str = str.substr(i);
		// std::cout << "new string = " << str << std::endl;
		total += to_add.size();
		// std::cout << "total = " << total << std::endl;
		if (total == number_of_bytes_in_data) {
			std::cout << "HERE" << std::endl;
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

	// std::cout << "remainder = " << remainder << std::endl;
	// std::cout << "INDEX = " << index << std::endl;
	while (1) {
		std::string hex_data_pair;
		hex_data_pair = handle_pair(str, index);
		// std::cout << "|" << hex_data_pair << "|" << std::endl;
		if (hex_data_pair.empty())
			break ;
		std::cout << "here" << std::endl;
		res.append(hex_data_pair);
	}
	// std::cout << "INDEX AFTER = " << index << std::endl;
	remainder.clear();
	remainder = str.substr(index);
	return (res);
}
