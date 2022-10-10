#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

typedef struct	Authority {
	std::string	user_information;
	std::string host;
	int			port;
}				Authority;

typedef struct	Uri
{
	std::string	scheme;
	Authority	authority;
	std::string	path;
	std::string file_name;
	std::string	query;
	std::string	fragment;
}				Uri;

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

void	check_uri(std::string const& str,  Uri& uri) {
	std::string	remainder = str;
	size_t		pos = remainder.find("://");
	
	if (pos != std::string::npos) {
		uri.scheme = remainder.substr(0, pos);
		std::cout << "URI SCHEME = " << uri.scheme << std::endl;
		remainder = remainder.substr(pos + 3);
		// std::cout << "remainder after scheme = " << remainder << std::endl;
	}
	pos = remainder.find("/");
	std::string authority = remainder.substr(0, pos);
	// std::cout << "Authority = " << authority << std::endl;
	if (pos != std::string::npos && !authority.empty()) {
		// std::cout << "HERE" << std::endl;
		size_t	pos2 = authority.find("@");
		if (pos2 != std::string::npos) {
			uri.authority.user_information = authority.substr(0, pos2);
			std::cout << "URI AUTHORITY USER INFORMATION = " << uri.authority.user_information << std::endl;
			authority = authority.substr(pos2 + 1, std::string::npos);
			// std::cout << "authority after user information " << authority << std::endl;
		}
		pos2 = authority.find(":");
		if (pos2 != std::string::npos) {
			// std::cout << pos2 << std::endl;
			uri.authority.host = authority.substr(0, pos2);
			std::cout << "URI AUTHORITY HOST = " << uri.authority.host << std::endl;
			authority = authority.substr(pos2 + 1);
			if (!authority.empty()) {
				uri.authority.port = stoi(authority);
				std::cout << "URI AUTHORITY PORT = " << uri.authority.port << std::endl;
			}
		}
		remainder = remainder.substr(pos + 1);
	}
	else {
		return ;
	}
	pos = remainder.find("?");
	if (pos != std::string::npos && !remainder.empty()) {
		uri.path = remainder.substr(0, pos);
	}
	else {
		pos = remainder.find("#");
		if (pos != std::string::npos && !remainder.empty()) {
			uri.path = remainder.substr(0, pos);
		}
		else {
			uri.path = remainder;
		}
	}
	std::cout << "END OF CHECK URI" << std::endl;	
}

void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

int main(int argc, char ** argv) {
	if (argc == 2) {
		std::string input;
		input = get_input_from_config_file(argv[1]);
		for (;;)
		{
			static int i = 0;
			Uri	uri;
			std::string line = input.substr(0, input.find("\r\n"));
			check_uri(line, uri);
			eraseAllSubStr(input, line);
			if (input.empty()) {
				std::cout << i << std::endl;
				break ;
			}
		}

	}
	return (0);
}