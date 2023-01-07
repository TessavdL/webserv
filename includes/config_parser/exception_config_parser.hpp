/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_config_parser.hpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:45:18 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/06 16:36:33 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_CONFIG_PARSER
# define EXCEPTION_CONFIG_PARSER

# include <string>
# include <stdexcept>

class ConfigException: public std::exception {
	public:
		ConfigException(void);
		ConfigException(std::string const message);
		ConfigException(ConfigException const& src);
		~ConfigException(void) throw();
		ConfigException& operator=(ConfigException const& other);
		const char *what(void) const throw();
	private:
		std::string	_message;
};

#endif
