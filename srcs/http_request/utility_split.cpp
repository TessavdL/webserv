/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_split.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:35:27 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:59:20 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request/utility_split.hpp"

std::string	string_until_deilimeter(std::string const& str, std::string const& delimeter) {
	std::string	ret;
	size_t		pos = str.find(delimeter);
	
	if (pos != std::string::npos) {
		ret = str.substr(0, pos);
	}
	return (ret);
}

std::vector<std::string>	split_string_on_delimeter_string(std::string str, std::string const& delimeter) {
	size_t						pos;
 	std::string					tmp;
	std::vector<std::string>	v;

	while ((pos = str.find(delimeter)) != std::string::npos) {
		tmp = str.substr(0, pos);
		v.push_back(tmp);
		str.erase(0, pos += delimeter.length());
	}
	if (!str.empty()) {
		v.push_back(str);
	}
	return (v);
}

std::vector<std::string>	split_string_on_delimeter(std::string const& str, char delimeter) {
	std::string					tmp;
	std::vector<std::string>	v;
	std::stringstream			ss(str);

	while (std::getline(ss, tmp, delimeter)) {
		v.push_back(tmp);
	}
	return (v);
}

std::pair<std::string, std::string>	split_string_in_half(std::string const& str, std::string delimeter) {
	size_t	pos = str.find(delimeter);
	std::string	str1;
	std::string	str2;

	if (pos != std::string::npos) {
		str1 = str.substr(0, pos);
		str2 = str.substr(pos + delimeter.length());
	}
	return (std::pair<std::string, std::string>(str1, str2));
}

//unused
std::pair<std::string, std::string>	split_string_in_half_on_any_match(std::string const& str, std::string delimeter) {
	size_t	pos = str.find_first_of(delimeter);
	std::string	str1;
	std::string	str2;

	if (pos != std::string::npos) {
		str1 = str.substr(0, pos);
		str2 = str.substr(pos + delimeter.length());
	}
	return (std::pair<std::string, std::string>(str1, str2));
}
