/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:19:50 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "./server_config.hpp"

class ServerConfig;
using namespace std;

class Location : protected ServerConfig
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
