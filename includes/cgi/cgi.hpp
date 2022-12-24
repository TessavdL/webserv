/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:29 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/24 12:21:22 by jelvan-d      ########   odam.nl         */
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
		void	create_argv(std::string const& file_location, Connection const& connection);
		void	create_env(Connection const& connection, RequestData const& request, std::string const& file_location);
		void	create_env_from_map(void);
		void	free_env_array(void);
		void	initiate_cgi_process(RequestData const& request);
		void	child_process(RequestData const& client);
		void	parent_process(RequestData const& request);
		void	get_content_from_cgi(void);
		string const&	get_body(void) const;
	private:
		map<string, string>	_env;
		string				_body;
		char**				_env_array;
		char*				_argv[3];
		pid_t				_pid;
		int					_fd[2][2];
};

#endif
