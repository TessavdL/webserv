/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/19 17:48:01 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "./server_config.hpp"

class server_config;
using namespace std;

class location : protected server_config
{
	private:
		string	_path;
	protected:
	public:
		location();
		location(location const& other);
		location &operator=(location const& rhs);
		virtual ~location();
};

#endif
