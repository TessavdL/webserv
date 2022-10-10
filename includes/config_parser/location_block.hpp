/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/10 15:54:29 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_BLOCK_HPP
# define LOCATION_BLOCK_HPP

# include "./server_config.hpp"
# include "./lexer.hpp"

using namespace std;

class LocationBlock : public ServerConfig
{
	private:
		vector<string>	_limit_except;
		string			_path_and_optional_modifier;
	protected:
	public:
		LocationBlock();
		LocationBlock(Lexer::t_locations location);
		LocationBlock(LocationBlock const& other);
		LocationBlock &operator=(LocationBlock const& rhs);
		virtual ~LocationBlock();
		void	get_directives(Lexer::t_locations location);
		string const&			get_path_and_optional_modifier() const;
		vector<string> const&	get_limit_except() const;
};

#endif
