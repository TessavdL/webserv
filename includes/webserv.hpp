/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:34:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 13:54:02 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "fatal_exception.hpp"
# include "./config_parser/server.hpp"
# include "./config_parser/lexer.hpp"
# include "./listening_sockets/socket_listen.hpp"
# include "./http_request/request_handler.hpp"
# include "./http_response/response_generator.hpp"

int	main(int ac, char **av);
int	kqueue_server(vector<Server>	server);

#endif
