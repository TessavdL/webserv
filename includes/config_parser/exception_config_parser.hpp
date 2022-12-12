/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_config_parser.hpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:45:18 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 14:02:55 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_CONFIG_PARSER
# define EXCEPTION_CONFIG_PARSER

# include <string>
# include <stdexcept>

class LexerParserException: public std::exception {
	public:
		LexerParserException(void);
		LexerParserException(std::string const message);
		LexerParserException(LexerParserException const& src);
		~LexerParserException(void) throw();
		LexerParserException& operator=(LexerParserException const& other);
		const char *what(void) const throw();
	private:
		std::string	_message;
};

#endif
