/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_split.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:35:27 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 17:40:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_request_parser/utility_split.hpp"



// send back two empty strings or fill first string with original str?

std::vector<std::string>	split_string_on_delimeter(std::string const& str, char delimiter) {
	std::string					tmp;
	std::vector<std::string>	v;
	std::stringstream			ss(str);

	while (std::getline(ss, tmp, delimiter)) {
		v.push_back(tmp);
	}
	return (v);
}

std::pair<std::string, std::string>	split_string_in_half(std::string const& str, std::string delimiter) {
	size_t	pos = str.find(delimiter);
	std::string	str1;
	std::string	str2;

	if (pos != std::string::npos) {
		str1 = str.substr(0, pos);
		str2 = str.substr(pos + delimiter.length());
	}
	return (std::pair<std::string, std::string>(str1, str2));
}

std::pair<std::string, std::string>	split_string_in_half_on_any_match(std::string const& str, std::string delimiter) {
	size_t	pos = str.find_first_of(delimiter);
	std::string	str1;
	std::string	str2;

	if (pos != std::string::npos) {
		str1 = str.substr(0, pos);
		str2 = str.substr(pos + delimiter.length());
	}
	return (std::pair<std::string, std::string>(str1, str2));
}
