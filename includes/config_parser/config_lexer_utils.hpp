/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config_lexer_utils.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/06 16:49:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:49:44 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_UTILS_HPP
# define LEXER_UTILS_HPP

# include "./exception_config_parser.hpp"

# include <string>
# include <algorithm>

# define EVEN 0
# define UNEVEN 1
# define WHITESPACE " \t\n\r\f\v"

bool		is_valid_server_start(std::string str);
bool		is_valid_location_start(std::string str);
void 		erase_substring(std::string& str, const std::string& to_erase);
std::string trim(std::string const& str, const char* to_trim);
std::string left_trim(std::string const& str, const char* to_trim);
std::string right_trim(std::string const& str, const char* to_trim);
size_t		count_blocks(std::string const& str);
bool 		check_valid_semicolumn(std::string const& str);
void		check_brackets(std::string str);
void		check_for_nested_location_blocks(std::string str);

#endif
