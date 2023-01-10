/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kqueue_and_kevent_utils.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/30 13:44:55 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 16:57:41 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/event_loop/kqueue_and_kevent_utils.hpp"

void	new_kernel_event_queue(int& kq) {
	kq = kqueue();
	if (kq == -1) {
		throw (FatalException("SYSCALL: kq in kqueue_server\n"));
	}
}

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

bool	is_new_connection(int event_identifier, map<int, vector<Server> > listening_sockets_with_config) {
	map<int, vector<Server> >::const_iterator it = listening_sockets_with_config.find(event_identifier);

	if (it != listening_sockets_with_config.end()) {
		return (true);
	}
	return (false);
}

void	add_write_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	add_new_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	add_read_event_to_kqueue(int kq, int event_fd) {
	struct kevent	monitor_event;

	EV_SET(&monitor_event, event_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	if (kevent(kq, &monitor_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in add_event_to_kqueue\n"));
	}
}

void	register_listening_sockets_to_kernel_events_kqueue(int const kq, map<int, vector<Server> > listening_sockets_with_config) {
	for (map<int, vector<Server> >::iterator it = listening_sockets_with_config.begin(); it != listening_sockets_with_config.end(); it++) {
		add_new_event_to_kqueue(kq, (*it).first);
	}
}

void	delete_read_event_from_kqueue(int kq, struct kevent *k_event, int event_fd) {
	EV_SET(k_event, event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(kq, k_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in delete_event_from_kqueue\n"));
	}
}

void	delete_write_event_from_kqueue(int kq, struct kevent *k_event, int event_fd) {
	EV_SET(k_event, event_fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if (kevent(kq, k_event, 1, NULL, 0, NULL) == -1) {
		throw (FatalException("SYSCALL: kevent in delete_event_from_kqueue\n"));
	}
}
