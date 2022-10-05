/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/04 17:50:53 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "./Server_Config.hpp"

class Server_Config;
using namespace std;

class Location : protected Server_Config
{
	private:
		string			_path;
		vector<string>	_limit_except;
		Location();
	protected:
	public:
		Location(t_locations locations);
		Location(Location const& other);
		Location &operator=(Location const& rhs);
		virtual ~Location();
		void	get_directives(t_locations locations);
};

#endif
