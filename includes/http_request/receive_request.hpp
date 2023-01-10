/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   receive_request.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 19:27:49 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 16:59:27 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECEIVE_REQUEST_HPP
# define RECEIVE_REQUEST_HPP

# include "../event_loop/kqueue_and_kevent_utils.hpp"
# include "../event_loop/connection.hpp"
# include "../http_response/error_checking.hpp"
# include "../http_request/request_handler.hpp"

# define BUFF_SIZE 4096

int	parse_request(Connection& client, std::string const& input);
int	receive_request(Connection& client, int connection_fd, int listen_backlog_size);
void	handle_request(Connection& client, int kq, struct kevent& event);

#endif
