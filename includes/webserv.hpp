/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:34:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/09 19:19:08 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//	favicon problems
//	delete cookies
//	https://stackoverflow.com/questions/19641000/favicon-is-not-loading-in-chrome

# define MAX_EVENTS 100

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/event.h>

# include "../includes/webserv.hpp"
# include "../includes/event_loop/connection.hpp"
# include "../includes/event_loop/kqueue_and_kevent_utils.hpp"
# include "../includes/http_response/prepare_response.hpp"
# include "../includes/http_request/receive_request.hpp"
# include "../includes/config_parser/server.hpp"
# include "../includes/config_parser/config_lexer.hpp"
# include "../includes/fatal_exception.hpp"

int		accept_connection(int event_fd);
void	add_connection(int event_fd, int connection_fd, map<int, Connection>& connections, map<int, vector<Server> > virtual_servers);
void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config);
int		event_loop(vector<Server> server);

#endif
