/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   event_loop_actions.hpp                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/11 16:09:22 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:42:27 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_LOOP_ACTIONS_HPP
# define EVENT_LOOP_ACTIONS_HPP

# include <map>
# include <vector>
# include <sys/event.h>
# include <sys/socket.h>
# include <sys/types.h>

# include "./connection_utils.hpp"
# include "./connection.hpp"
# include "./kqueue_and_kevent_utils.hpp"
# include "../config_parser/server.hpp"
# include "../http_response/response_generator.hpp"
# include "../http_response/response_handler.hpp"
# include "../fatal_exception.hpp"

# define BUFF_SIZE 4096
# define REQUEST_EXCEPTION -2

void	handle_disconnect(std::map<int, Connection>& connections, int const event_fd);
void	handle_new_connection(std::map<int, Connection>& connections, std::map<int, std::vector<Server> > const& listening_sockets_with_config, int const kq, int const event_fd);
void	handle_readable_event(std::map<int, Connection>& connections, int const kq, struct kevent& event);
void	handle_writable_event(std::map<int, Connection>& connections, int const kq, struct kevent& event);

#endif
