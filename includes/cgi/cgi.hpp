/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:29 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/14 16:08:47 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../event_loop/connection.hpp"
# include "../config_parser/server.hpp"

class Cgi {
	public:
		Cgi();
		Cgi(Connection const& connection);
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& rhs);
		~Cgi();
		void	create_env(Connection const& connection, Connection::t_request const& request);
	private:
		map<string, string>	_env;
		pid_t				_pid;
		int					_fd[2][2];
};

#endif
