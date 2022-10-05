/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 13:42:53 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/14 14:43:03 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_HPP
# define PARSER_HPP

# include <string>

class Parser
{
	private:

	protected:
		
	public:
		Parser();
		Parser(Parser const &other);
		Parser &operator=(Parser const& rhs);
		virtual~Parser();
};

#endif
