/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 16:44:24 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/13 18:52:35 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

#include "lexer_utils.hpp"
#include "./exception_config_parser.hpp"

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
		Lexer(std::string const& file_name);
		~Lexer();
		Lexer(Lexer const& other);
		Lexer&												operator=(Lexer const& other);
		std::string											get_input_from_config_file(std::string const& file_name) const;
		void												check_brackets(std::string str);
		std::pair<size_t, size_t>							get_start_and_end_of_block(std::string str);
		std::vector<std::string>							get_server_blocks(std::string const& input);
		std::vector<std::pair<std::string,std::string> >	get_location_blocks(std::string const& server_block);
		void												get_directives(std::vector<std::string>& directives, std::string const& str);
		std::vector<t_server>								get_server_blocks(void) const;
	protected:
		std::vector<t_server>								server_blocks;
	private:
		Lexer();
};

std::ostream&	operator<<(std::ostream& os, Lexer const& lexer);

#endif
