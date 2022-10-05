/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 17:52:54 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "./server_config.hpp"
# include "./lexer.hpp"

class ServerConfig;
using namespace std;

class Location : public ServerConfig
{
	private:
		string			_path_and_optional_modifier;
		vector<string>	_limit_except;
	protected:
	public:
		Location();
		Location(Lexer::t_locations locations);
		Location(Location const& other);
		Location &operator=(Location const& rhs);
		virtual ~Location();
		void	get_directives(Lexer::t_locations locations);
};

#endif
