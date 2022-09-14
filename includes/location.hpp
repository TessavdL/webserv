/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/14 18:11:06 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "./server_config.hpp"

class server_config;

class location : protected server_config
{
	private:
		
	public:
		location();
		location(location const& other);
		location &operator=(location const& rhs);
		virtual ~location();
};

#endif
