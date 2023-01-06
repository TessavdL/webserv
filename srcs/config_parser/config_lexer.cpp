/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_lexer.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/06 16:41:43 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:48:21 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/config_parser/config_lexer.hpp"

ConfigLexer::ConfigLexer(std::string const& file_name) {
	std::string const	input = get_input_from_config_file(file_name);

	check_brackets(input);
	check_for_nested_location_blocks(input);

	size_t const	n_server_blocks = count_blocks(input);
	
	if (n_server_blocks == 0) {
		throw (ConfigException("Empty server blocks"));
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

ConfigLexer::~ConfigLexer(void) {

}

ConfigLexer::ConfigLexer(ConfigLexer const& other) {
	*this = other;
}

ConfigLexer&	ConfigLexer::operator=(ConfigLexer const& other) {
	if (this != &other) {
		this->server_blocks = other.server_blocks;
	}
	return (*this);
}

std::string	const	ConfigLexer::get_input_from_config_file(std::string const& file_name) const {
	std::ifstream	input_stream;
	std::string		input;
	
	input_stream.open(file_name);
	if (!input_stream || input_stream.bad()) {
		throw (ConfigException("Failed to open the config file"));
	}
	else {
		std::ostringstream ss;
		ss << input_stream.rdbuf();
		input.append(ss.str());
	}
	input_stream.close();
	return (input);
}

std::pair<size_t, size_t>	ConfigLexer::get_start_and_end_of_block(std::string const& str) const {
	size_t const	open = str.find("{");

	if (open == std::string::npos) {
		throw (ConfigException("Invalid start of block"));
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
	throw (ConfigException("Invalid start and end of block"));
}

std::vector<std::string>	ConfigLexer::get_server_blocks(std::string const& input) const {
	std::string					str = input;
	std::string					server_block;
	std::vector<std::string>	server_blocks;
	std::pair<size_t, size_t>	start_end;
	size_t const				n_server_blocks = count_blocks(str);

	for (size_t i = 0; i < n_server_blocks; i++) {
		start_end = get_start_and_end_of_block(str);
		server_block = str.substr(start_end.first + 1, start_end.second - start_end.first - 2);
		if (!is_valid_server_start(str.substr(0, start_end.first))) {
			throw (ConfigException("Invalid server block start"));
		}
		server_blocks.push_back(server_block);
		erase_substring(str, str.substr(0, start_end.second));
	}
	return (server_blocks);
}

static std::string const	get_location(std::string const& str, size_t end) {
	std::string	location = str.substr(0, end);
	size_t		pos = location.find_last_of(";");

	if (pos == std::string::npos) {
		pos = 0;
	}
	else {
		pos++;
	}
	location = location.substr(pos, location.size());
	return (location);
}

std::vector<std::pair<std::string,std::string> >	ConfigLexer::get_location_blocks(std::string const& server_block) const {
	std::string											str = server_block;
	std::vector<std::pair<std::string,std::string> >	location_blocks;
	std::pair<size_t, size_t>							start_end;
	size_t const										n_location_blocks = count_blocks(str);

	for (size_t i = 0; i < n_location_blocks; i++) {
		start_end = get_start_and_end_of_block(str);
		std::string location_block = str.substr(start_end.first + 1, start_end.second - start_end.first - 2);
		std::string location = trim(get_location(str, start_end.first), WHITESPACE);
		if (!is_valid_location_start(location)) {
			throw (ConfigException("Invalid location block start"));
		}
		location_blocks.push_back(std::make_pair(location, location_block));
		erase_substring(str, str.substr(0, start_end.second));
	}
	return (location_blocks);
}

static void	add_directive(std::string const& str, std::vector<std::string>& directives) {
	std::string directive = trim(str, WHITESPACE);

	if (directive.empty()) {
		return ;
	}
	if (directive.back() != ';' || !check_valid_semicolumn(directive)) {
		throw (ConfigException("Invalid directive, every directive should end with a \';\' and a newline"));
	}
	directive = directive.substr(0, directive.size() - 1);
	if (!directive.empty())
		directives.push_back(directive);
}

void	ConfigLexer::get_directives(std::vector<std::string>& directives, std::string const& str) const {
	size_t		pos;
	std::string	directive;
	std::string	remainder;
	
	for (size_t i = 0; i < str.size(); i++) {
		remainder = str.substr(i);
		pos = remainder.find_first_of('\n');

		if (pos == std::string::npos) {
			break ;
		}
		directive = remainder.substr(0, pos);
		if (directive.empty()) {
			continue ;
		}
		if (directive.find('{') != std::string::npos) {
			i += remainder.find('}');
			continue ;
		}
		i += directive.size();
		add_directive(directive, directives);
	}
}

std::vector<ConfigLexer::t_server> const&	ConfigLexer::get_server_blocks(void) const {
	return (this->server_blocks);
}

ConfigLexer::ConfigLexer(void) {

}

std::ostream&	operator<<(std::ostream& os, ConfigLexer const& lexer) {
	std::vector<ConfigLexer::t_server> const server_block = lexer.get_server_blocks();

	for (size_t i = 0; i < server_block.size(); i++) {
		for (size_t n = 0; n < server_block[i].directives.size(); n++) {
			os << server_block[i].directives[n] << std::endl;
		}

		std::vector<ConfigLexer::t_locations>	const location_blocks = server_block[i].locations;

		for (size_t j = 0; j < location_blocks.size(); j++) {
			os << location_blocks[j].path_and_optional_modifier << std::endl;
			for (size_t m = 0; m < location_blocks[j].directives.size(); m++) {
				os << "\t" << location_blocks[j].directives[m] << std::endl;
			}
		}
	}
	return (os);
}
