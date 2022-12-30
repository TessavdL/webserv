/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_utils.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 13:45:24 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/30 13:48:39 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQUEUE_UTILS_HPP
# define KQUEUE_UTILS_HPP

# include <map>
# include <sys/event.h>
# include <vector>

# include "./config_parser/server.hpp"

bool	is_event_error(u_short event_flag);
bool	client_disconnected(u_short event_flag);
bool	is_readable_event(short event_filter);
bool	is_writable_event(short event_filter);
bool	event_identifier_equals_listening_socket_fd(int event_identifier, map<int, vector<Server> > listening_sockets_with_config);
bool	is_new_connection(int event_identifier, map<int, vector<Server> > listening_sockets_with_config);

#endif
