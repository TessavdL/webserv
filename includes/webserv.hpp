/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:34:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/11 15:13:33 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <netinet/in.h>
# include <sys/socket.h>
# include <sys/event.h>

# include "./config_parser/config_lexer.hpp"
# include "./config_parser/server.hpp"
# include "./event_loop/connection_utils.hpp"
# include "./event_loop/connection.hpp"
# include "./event_loop/kqueue_and_kevent_utils.hpp"
# include "./http_response/prepare_response.hpp"
# include "./http_request/receive_request.hpp"
# include "./fatal_exception.hpp"

# define MAX_EVENTS 100

void	create_listening_sockets_with_config(vector<Server> server, map<int, vector<Server> >& listening_sockets_with_config);
int		event_loop(vector<Server> server);

#endif
