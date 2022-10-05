/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Lexer.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 15:02:14 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:07:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/lexer.hpp"

Lexer::Lexer(std::string const& file_name) {
	std::string		input = get_input_from_config_file(file_name);
	size_t			n_server_blocks = count_blocks(input);
	
	if (n_server_blocks == 0) {
		throw (LexerParserException());
	}

	std::vector<std::string>	server_block_rough = get_server_blocks(input);

	for (size_t i = 0; i < n_server_blocks; i++) {
		t_server											server_block;
		size_t												n_locations_blocks = count_blocks(server_block_rough[i]);
		std::vector<std::pair<std::string,std::string> >	location_blocks_rough = get_location_blocks(server_block_rough[i]);

		get_directives(server_block.directives, server_block_rough[i]);
		for (size_t j = 0; j < n_locations_blocks; j++) {
			t_locations	locations;
			locations.path_and_optional_modifier = location_blocks_rough[j].first;
			get_directives(locations.directives, location_blocks_rough[j].second);
			server_block.locations.push_back(locations);
		}
		this->server_blocks.push_back(server_block);
	}
}

Lexer::~Lexer(void) {}

Lexer::Lexer(Lexer const& other) : server_blocks(other.server_blocks) {}

Lexer&	Lexer::operator=(Lexer const& other) {
	if (this != &other) {
		this->server_blocks = other.server_blocks;
	}
	return (*this);
}

std::string	Lexer::get_input_from_config_file(std::string const& file_name) const {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open(file_name);
	if (!input_stream) { // open failed
		throw (LexerParserException());
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		input.append(ss.str());
	}
	input_stream.close();
	return (input);
}

std::pair<size_t, size_t>	Lexer::get_start_and_end_of_block(std::string str) {
	size_t	open = str.find("{");
	if (open == std::string::npos) {
		throw (LexerParserException());
	}
	size_t	depth = 1;
	for (size_t i = 0; i + open + 1 < str.size(); i++) {
		if (str[i + open + 1] == '{') {
			depth++;
		}
		else if (str[i + open + 1] == '}') {
			depth--;
		}
		if (depth == 0) {
			return (std::pair<size_t, size_t>(open, i + open + 2));
		}
	}
	throw (LexerParserException());
}

std::vector<std::string>	Lexer::get_server_blocks(std::string const& input) {
	std::string					str = input;
	std::string					server_block;
	std::vector<std::string>	server_blocks;
	std::pair<size_t, size_t>	start_end;
	size_t						n_server_blocks;

	n_server_blocks = count_blocks(str);
	for (size_t i = 0; i < n_server_blocks; i++) {
		start_end = get_start_and_end_of_block(str);
		server_block = str.substr(start_end.first + 1, start_end.second - start_end.first - 2);
		if (!is_valid_server_start(str.substr(0, start_end.first))) {
			throw (LexerParserException());
		}
		server_blocks.push_back(server_block);
		erase_substring(str, str.substr(0, start_end.second));
	}
	return (server_blocks);
}

static std::string	get_location(std::string const& str, size_t end) {
	std::string	location;
	size_t 		start;
	
	location = str.substr(0 , end);
	start = location.find_last_of(";");
	if (start == std::string::npos) {
		start = 0;
	}
	else {
		start++;
	}
	location = location.substr(start, location.size());
	return (location);
}

std::vector<std::pair<std::string,std::string> >	Lexer::get_location_blocks(std::string const& server_block) {
	std::string											str = server_block;
	std::vector<std::pair<std::string,std::string> >	location_blocks;
	std::string											path;
	std::pair<size_t, size_t>							start_end;
	size_t												n_location_blocks;

	n_location_blocks = count_blocks(str);
	for (size_t i = 0; i < n_location_blocks; i++) {
		start_end = get_start_and_end_of_block(str);
		std::string location_block = str.substr(start_end.first + 1, start_end.second - start_end.first - 2);
		std::string location = trim(get_location(str, start_end.first), " \t\n\r\f\v");
		location_blocks.push_back(std::pair<std::string, std::string>(location, location_block));
		erase_substring(str, str.substr(0, start_end.second));
	}
	return (location_blocks);
}

void	Lexer::get_directives(std::vector<std::string>& directives, std::string const& str) {
	std::string	directive;
	
	for (size_t i = 0; i < str.size(); i++) {
		std::string remainder = str.substr(i, str.size());
		directive = remainder.substr(0, remainder.find_first_of(';'));
		if (directive.find('{') != std::string::npos) {
			i += remainder.find('}');
			continue ;
		}
		i += directive.size();
		directives.push_back(trim(directive, " \t\n\r\f\v"));
	}
}

std::vector<Lexer::t_server>	Lexer::get_server_blocks(void) const {
	return (this->server_blocks);
}

Lexer::Lexer(void) {}

std::ostream&	operator<<(std::ostream& os, Lexer const& lexer) {
	std::vector<Lexer::t_server> serv = lexer.get_server_blocks();

	for (size_t i = 0; i < serv.size(); i++) {
		for (size_t n = 0; n < serv[i].directives.size(); n++) {
			os << serv[i].directives[n] << std::endl;
		}

		std::vector<Lexer::t_locations>	locs = serv[i].locations;

		for (size_t j = 0; j < locs.size(); j++) {
			os << locs[j].path_and_optional_modifier << std::endl;
			for (size_t m = 0; m < locs[j].directives.size(); m++) {
				os << "\t" << locs[j].directives[m] << std::endl;
			}
		}
	}
	return (os);
}
