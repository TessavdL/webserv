/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:35:31 by jelvan-d      #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2022/10/12 20:28:32 by jelvan-d      ########   odam.nl         */
=======
/*   Updated: 2022/10/13 14:12:02 by tevan-de      ########   odam.nl         */
>>>>>>> 8e6ea91af557481910aaa6edc60889eef1129436
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "./utility_split.hpp"

#define RESERVED_CHARACTERS "!#$&'()*+,/:;=?@[]"
#define RESERVED_CHARACTERS_MINUS_FORWARD_SLASH "!#$&'()*+,:;=?@[]"

typedef struct							Authority {
	std::string							user_information;
	std::string 						host;
	int									port;
}										Authority;

typedef struct 							Path
{
	std::string							full;
	std::string							without_extension;
	std::string							extension;
}										Path;

typedef struct							Uri
{
	std::string							scheme;
	Authority							authority;
	Path								path;
	std::string 						file_name;
	std::map<std::string, std::string>	query;
	std::string							fragment;
}										Uri;

std::string	get_input_from_config_file(std::string const& file_name) {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open(file_name);
	if (!input_stream) { // open failed
		std::cout << "OPEN FAILED" << std::endl;
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		input.append(ss.str());
	}
	input_stream.close();
	return (input);
}

void	check_scheme(std::string const& scheme_str, std::string& scheme, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(scheme_str.substr(index), "://");
	scheme = p.first;
	if (!scheme.empty()) {
		std::cout << "SCHEME = " << scheme << std::endl;
		index += scheme.length() + 3;
	}
}

void	check_authority(std::string const& authority_str, Authority& authority, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(authority_str.substr(index), "/");
	std::string							str = p.first;

	if (!str.empty()) {
		std::pair<std::string, std::string>	p2 = split_string_in_half(str, "@");
		std::string							str2 = p2.first;
		authority.user_information = str2;
		if (!authority.user_information.empty()) {
			std::cout << "USER INFORMATION = " << authority.user_information << std::endl;
			str2 = p2.second;
			index += (authority.user_information.length() + 1);
		}
		else { // if no @ is found then continue forward with the original string
			str2 = str;
		}
		
		std::pair<std::string, std::string>	p3 = split_string_in_half(str2, ":");
		std::string							port;
		authority.host = p3.first;
		if (!authority.host.empty()) {
			std::cout << "AUTHORITY HOST = " << authority.host << std::endl;
			index += (authority.host.length() + 1);
		}
		else {
			authority.host = str2;
			if (!authority.host.empty()) {
				std::cout << "AUTHORITY HOST = " << authority.host << std::endl;
				index += (authority.host.length() + 1);
			}
		}
		port = p3.second;
		if (!port.empty()) {
			authority.port = stoi(port);
			index += (port.length() + 1);
			std::cout << "AUTHORITY PORT = " << authority.port << std::endl;
		}
		else {
			authority.port = -1;
			std::cout << "AUTHORITY PORT = " << authority.port << std::endl;
		}
	}
}

// should think of a way to handle multiple "//" after each other

void	check_path(std::string const& path_str, Path& path, size_t& index) {
	std::string							substring = path_str.substr(index);
	std::pair<std::string, std::string>	p = split_string_in_half_on_any_match(substring, "?#");
	std::string							str = p.first;

	if (!p.first.empty()) {
		path.full = p.first;
		std::cout << "PATH FULL = " << path.full << std::endl;
	}
	else {
		path.full = substring;
		std::cout << "PATH FULL = " << path.full << std::endl;
	}
	index += path.full.length();
	std::pair<std::string, std::string>	p2 = split_string_in_half(path.full, ".");
	if (!p2.second.empty()) {
		path.without_extension = p2.first;
		path.extension = p2.second;
		std::cout << "PATH WITHOUT EXTENSION = " << path.without_extension << std::endl;
		std::cout << "PATH EXTENSION = " << path.extension << std::endl;
	}
}

void	check_query(std::string const& query_str, std::map<std::string, std::string>& query, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(query_str.substr(index), "?");
	std::string							str = p.second;

	if (!str.empty()) {
		std::pair<std::string, std::string>	p2 = split_string_in_half(str, "#");
		std::string							str2 = p2.first;
		if (str2.empty()) {
			str2 = str;
			index += str2.length();
		}
		else {
			index += str2.length() + 1;
		}
		std::vector<std::string>	query_seperated = split_string_on_delimeter(str2, '&');
		for (size_t i = 0; i < query_seperated.size(); i++) {
			if (!query_seperated[i].empty()) {
				std::pair<std::string, std::string> p = split_string_in_half(query_seperated[i], "=");
				query.insert(p);
			}
		}
	}
}

void	check_fragment(std::string const& fragment_str, std::string& fragment, size_t& index) {
	std::pair<std::string, std::string>	p = split_string_in_half(fragment_str.substr(index), "#");
	std::string							str = p.second;

	if (!str.empty()) {
		fragment = str;
		std::cout << "FRAGMENT = " << fragment << std::endl;
		index += fragment.length() + 1;
	}
}

void	check_uri(std::string const& str,  Uri& uri) {
	size_t		index = 0;
	
	if (str.length() > 2000) {
		std::cout << "ERROR\nuri too long\n" << std::endl;
		return ;
	}
	std::cout << "URI = " << str << std::endl;
	check_scheme(str, uri.scheme, index);
	check_authority(str, uri.authority, index);
	check_path(str, uri.path, index);
	if (str.substr(index).empty()) {
		std::cout << std::endl;
		return ;
	}
	check_query(str, uri.query, index);
	if (!uri.query.empty()) {
		std::cout << "QUERY STRING = ";
		for (std::map<std::string, std::string>::iterator it = uri.query.begin(); it != uri.query.end(); it++) {
			std::cout << (*it).first << "=" << (*it).second << ",";
		}
		std::cout << std::endl;
	}
	if (str.substr(index).empty()) {
		std::cout << std::endl;
		return ;
	}
	else {
		check_fragment(str, uri.fragment, index);
	}
	std::cout << std::endl;	
}

// void	HTTPRequestParser::check_uri(std::string const& uri) {
// 	std::string	remainder = uri;
// 	size_t		pos = remainder.find("://");
	
// 	if (pos != std::string::npos) {
// 		this->_uri.scheme = remainder.substr(0, pos);
// 		std::cout << "URI SCHEME = " << this->_uri.scheme << std::endl;
// 		remainder = remainder.substr(pos + 3);
// 	}
// 	pos = remainder.find("/");
// 	std::string authority = remainder.substr(0, pos);
// 	if (pos != std::string::npos && !authority.empty()) {
// 		size_t	pos2 = authority.find("@");
// 		if (pos != std::string::npos) {
// 			this->_uri.authority.user_information = authority.substr(0, pos2);
// 			std::cout << "URI AUTHORITY USER INFORMATION = " << this->_uri.authority.user_information << std::endl;
// 			authority = authority.substr(pos + 1);
// 		}
// 		pos2 = authority.find(":");
// 		if (pos2 != std::string::npos) {
// 			this->_uri.authority.host = authority.substr(0, pos2);
// 			authority = authority.substr(pos + 1);
// 			if (!authority.empty())
// 				this->_uri.authority.port = stoi(authority);
// 		}
// 		remainder = remainder.substr(pos + 1);
// 	}
// 	else {
// 		return ;
// 	}
// 	pos = remainder.find("?");
// 	if (pos != std::string::npos && !remainder.empty()) {
// 		this->_uri.path = remainder.substr(0, pos);
// 	}
// 	else {
// 		pos = remainder.find("#");
// 		if (pos != std::string::npos && !remainder.empty()) {
// 			this->_uri.path = remainder.substr(0, pos);
// 		}
// 		else {
// 			this->_uri.path = remainder;
// 		}
// 	}
// 	std::cout << "END OF CHECK URI" << std::endl;	
// }


void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        mainStr.erase(pos, toErase.length());
    }
}

int main(int argc, char ** argv) {
	if (argc == 2) {
		std::string input;
		input = get_input_from_config_file(argv[1]);
		std::cout << "INPUT = " << std::endl << input << std::endl;
		for (;;)
		{
			Uri	uri;
			size_t pos = input.find("\n");
			std::string line = input.substr(0, pos);
			check_uri(line, uri);
			line += '\n';
			eraseAllSubStr(input, line);
			if (input.empty()) {
				break ;
			}
		}
	}
	return (0);
}
