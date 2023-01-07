/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:34:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/07 15:24:37 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define MAX_EVENTS 100

# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/event.h>
# include <iostream>

# include "../includes/webserv.hpp"
# include "../includes/event_loop/connection.hpp"
# include "../includes/http_response/error_checking.hpp"
# include "../includes/http_response/response_handler.hpp"
# include "../includes/kqueue_utils.hpp"
# include "../includes/http_request/receive_request.hpp"
# include "../includes/config_parser/server.hpp"
# include "../includes/config_parser/config_lexer.hpp"
# include "../includes/fatal_exception.hpp"
# include "../includes/http_response/response_generator.hpp"

int		main(int ac, char **av);
int		kqueue_server(vector<Server> server);
int		accept_connection(int event_fd);
void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config);
void	add_connection(int event_fd, int connection_fd, map<int, Connection>& connections, map<int, vector<Server> > virtual_servers);
void	send_response_to_client(int connection_fd, Connection& client);

#endif
