/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_listen.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 16:44:37 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/31 14:14:07 by jelvan-d      ########   odam.nl         */
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
		SocketListen&				operator=(SocketListen const& other);
		void						create_socket(void);
		void						define_socket_address(void);
		void						bind_socket_to_address(void);
		void						listen_on_socket(void);
		int							get_port_number_from_socket_fd(void) const;
		struct sockaddr_in const&	get_addr(void) const;
		int const&					get_fd(void) const;
		int	const&					get_port_number(void) const;
	private:
		struct sockaddr_in	_addr;
		int					_fd;
		int					_port_number;
		SocketListen();
		
};

# endif
