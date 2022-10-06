/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_listen.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 16:44:37 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:44:39 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_LISTEN_HPP
# define SOCKET_LISTEN_HPP

# include "../fatal_exception.hpp"

# include <fcntl.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>

class SocketListen {
	public:
		SocketListen(int port_number);
		~SocketListen();
		SocketListen(SocketListen const& other);
		SocketListen&		operator=(SocketListen const& other);
		void				create_socket(void);
		void				make_address_reusable(void);
		void				define_socket_address(void);
		void				bind_socket_to_address(void);
		void				listen_on_socket(void);
		struct sockaddr_in	getAddr(void) const;
		int					getFd(void) const;
		int					getPortNumber(void) const;
	private:
		struct sockaddr_in	_addr;
		int					_fd;
		int					_port_number;
		SocketListen();
		
};

# endif
