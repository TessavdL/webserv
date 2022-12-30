/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_utils.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 13:44:55 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/30 13:45:23 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/kqueue_utils.hpp"

bool	is_event_error(u_short event_flag) {
	if (event_flag & EV_ERROR) {
		return (true);
	}
	return (false);
}

bool	client_disconnected(u_short event_flag) {
	if (event_flag & EV_EOF) {
		return (true);
	}
	return (false);
}

bool	is_readable_event(short event_filter) {
	if (event_filter == EVFILT_READ) {
		return (true);
	}
	return (false);
}

bool	is_writable_event(short event_filter) {
	if (event_filter == EVFILT_WRITE) {
		return (true);
	}
	return (false);
}

bool	event_identifier_equals_listening_socket_fd(int event_identifier, map<int, vector<Server> > listening_sockets_with_config) {
	map<int, vector<Server> >::const_iterator it = listening_sockets_with_config.find(event_identifier);

	if (it != listening_sockets_with_config.end()) {
		return (true);
	}
	return (false);
}

bool	is_new_connection(int event_identifier, map<int, vector<Server> > listening_sockets_with_config) {
	if (event_identifier_equals_listening_socket_fd(event_identifier, listening_sockets_with_config)) {
		return (true);
	}
	return (false);
}
