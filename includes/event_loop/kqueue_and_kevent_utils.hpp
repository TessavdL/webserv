/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_and_kevent_utils.hpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 13:45:24 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 16:58:35 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQUEUE_AND_KEVENT_UTILS
# define KQUEUE_AND_KEVENT_UTILS

# include <map>
# include <sys/event.h>
# include <vector>

# include "../config_parser/server.hpp"

void	new_kernel_event_queue(int& kq);

bool	is_event_error(u_short event_flag);
bool	client_disconnected(u_short event_flag);
bool	is_readable_event(short event_filter);
bool	is_writable_event(short event_filter);

bool	is_new_connection(int event_identifier, map<int, vector<Server> > listening_sockets_with_config);

void	add_write_event_to_kqueue(int kq, int event_fd);
void	add_new_event_to_kqueue(int kq, int event_fd);
void	add_read_event_to_kqueue(int kq, int event_fd);

void	delete_read_event_from_kqueue(int kq, struct kevent *k_event, int event_fd);
void	delete_write_event_from_kqueue(int kq, struct kevent *k_event, int event_fd);

void	register_listening_sockets_to_kernel_events_kqueue(int const kq, map<int, vector<Server> > listening_sockets_with_config);

#endif
