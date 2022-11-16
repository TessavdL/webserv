/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:29 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/16 17:21:37 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "../event_loop/connection.hpp"
# include "../config_parser/server.hpp"
# include "../listening_sockets/socket_listen.hpp"
# include <unistd.h>

class Cgi {
	public:
		Cgi();
		Cgi(Connection const& connection, std::string const& file_location);
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& rhs);
		~Cgi();
		void	create_argv(void);
		void	create_env(Connection const& connection, Connection::t_request const& request, std::string const& file_location);
		void	create_env_from_map(void);
		void	free_env_array(void);
		void	initiate_cgi_process(void);
	private:
		map<string, string>	_env;
		char**				_env_array;
		char*				_argv[3];
		pid_t				_pid;
		// int					_fd[2][2];
};

#endif
