/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:34:50 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/23 13:47:24 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "fatal_exception.hpp"
# include "./config_parser/server.hpp"
# include "./config_parser/lexer.hpp"
# include "./listening_sockets/socket_listen.hpp"
# include "./http_request_parser/http_request_lexer.hpp"
# include "./http_request_parser/http_request_parser.hpp"
# include "./http_response/response.hpp"

int	main(int ac, char **av);
int	kqueue_server(vector<Server>	server);

#endif
