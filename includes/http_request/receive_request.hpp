/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   receive_request.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 19:27:49 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/30 20:07:12 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_REQUEST_HPP
# define RECEIVE_REQUEST_HPP

# include "../../includes/event_loop/connection.hpp"
# include "../../includes/http_response/error_checking.hpp"
# include "../../includes/http_request/request_handler.hpp"

# define BUFF_SIZE 4096

int		parse_request(Connection& client, std::string const& input);
void	receive_request(Connection& client, int connection_fd, int listen_backlog_size);

#endif
