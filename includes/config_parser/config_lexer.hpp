/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_lexer.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/06 16:49:48 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:49:54 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
# define LEXER_HPP

# include "./config_lexer_utils.hpp"
# include "./exception_config_parser.hpp"

# include <algorithm>
# include <fstream>
# include <functional>
# include <iostream>
# include <sstream>
# include <string>
# include <vector>

class ConfigLexer {
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
		ConfigLexer(std::string const& file_name);
		~ConfigLexer();
		ConfigLexer(ConfigLexer const& other);
		ConfigLexer&										operator=(ConfigLexer const& other);
		std::string	const									get_input_from_config_file(std::string const& file_name) const;
		std::pair<size_t, size_t>							get_start_and_end_of_block(std::string const& str) const;
		std::vector<std::string>							get_server_blocks(std::string const& input) const;
		std::vector<std::pair<std::string,std::string> >	get_location_blocks(std::string const& server_block) const;
		void												get_directives(std::vector<std::string>& directives, std::string const& str) const;
		std::vector<t_server> const&						get_server_blocks(void) const;
	protected:
		std::vector<t_server>								server_blocks;
	private:
		ConfigLexer();
};

std::ostream&	operator<<(std::ostream& os, ConfigLexer const& lexer);

#endif
