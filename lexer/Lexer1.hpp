#ifndef LEXER_HPP
# define LEXER_HPP

#include "LexerUtils.hpp"
#include "Exception.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <functional>

class Lexer {
	public:
		typedef struct					s_locations
		{
			std::string					path_and_optional_modifier;
			std::vector<std::string>	directives;
		}								t_locations;
		typedef struct					s_server
		{
			std::vector<std::string>	directives;
			std::vector<t_locations>	locations;
		}								t_server;
		Lexer();
		~Lexer();
		Lexer(Lexer const& other);
		Lexer&						operator=(Lexer const& other);
		std::string					get_input_from_config_file(std::string const& file_name) const;
		void						check_brackets(std::string str);
		std::pair<size_t, size_t>	get_start_and_end_of_server_block(std::string str);
		std::vector<std::string>	get_server_blocks(std::string const& input);
	protected:
		std::vector<t_server>		server_blocks;
};

#endif
