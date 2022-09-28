#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// close

#include <sys/types.h>
#include <sys/socket.h>	// socket, bind, listen, accept
#include <sys/event.h>	// kqueue?

#include <netinet/in.h> // contains sockaddr_in struct we need for address and port and htons, htonl, ntohl, ntohs

#include <string>
#include <vector>
#include <iostream>

int	ft_strlen(char *s)
{
	int	i;
	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int	main(void)
{
	char	server_message[256] = "You have reached this server.";

	//	create socket (see client for more)
	int		server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		return (error_and_exit("An error occured in socket.\n"));
	}

	//	setsockopt, make socket address reusable
	int yes=1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	//	define server address (see client for more);
	struct sockaddr_in	server_socket_address;
	server_socket_address.sin_family = AF_INET;
	server_socket_address.sin_port = htons(9002);
	server_socket_address.sin_addr.s_addr = INADDR_ANY;

	//	bind
	//	binds the socket to the specified IP address and port
	//	assigning a name to a socket
	//	int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	int	bind_status;
	bind_status = bind(server_socket, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));
	if (bind_status == -1)
	{
		return (error_and_exit("An error occured in bind.\n"));
	}
	
	//	listen
	//	int listen(int sockfd, int backlog);
	//	sockfd is a file descriptor that refers to a socket of type SOCK_STREAM or SOCK_SEQPACKET
	//	in our case: SOCKSTREAM
	//	backlog defines the maximum length to which the queue of pending connections for sockfd may grow
	//	if a connection request arrives when the queue is full the client can receive an error or ignored until later
	int	listen_status;
	listen_status = listen(server_socket, 5);
	if (listen_status == -1)
	{
		return (error_and_exit("An error occured in listen.\n"));
	}

	// struct kevent	change_event[4];
	// struct kevent	event[4];
	// int				new_events;
	// int				kq = kqueue();

	// EV_SET(change_event, server_socket, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

	// if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
	// 	return (error_and_exit("An error occured in kevent"));
	// }

	// printf("Event loop starting...\n");
	// //	event loop
	// for (;;) {
	// 	new_events = kevent(kq, NULL, 0, event, 1, NULL);
	// 	if (new_events == -1) {
	// 		return (error_and_exit("An error occured in kevent in the event loop"));
	// 	}
	
	// 	for (int i = 0; new_events > i; i++) {
	// 		int	event_fd = event[i].ident;
	// 		if (event[i].flags & EOF) {
	// 			printf("Client disconnected");
	// 			if (close(event_fd) == -1) {
	// 				return (error_and_exit("An error occured in close"));
	// 			}
	// 		}
	// 		else if (event_fd == server_socket) {
				// int	client_socket;
				// client_socket = accept(server_socket, NULL, NULL);
				// if (client_socket == -1) {
				// 	return (error_and_exit("An error occured in accept.\n"));
				// }
	// 			printf("A client connected to the server\n");
	// 			EV_SET(change_event, client_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
	// 			if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
	// 				return (error_and_exit("An error occured in kevent in event loop registering new event"));
	// 			}
	// 		}
	// 		else if (event[i].filter & EVFILT_READ) {
	// 			// read bytes from client socket
	// 			char buf[1024];
	// 			size_t	bytes_read = recv(event_fd, buf, sizeof(buf), 0);
	// 			printf("read %zu bytes\n", bytes_read);
	// 		}
	// 	}
	// }


	//	accept
	//	int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
	//	integer to hold client socket to which we can write
	//	used with connection-based socket types SOCK_STREAM and SOCK_SEQPACKET, we use SOCK_STREAM
	//	allows us to manipulate anything on the client side
	//	two-way connection is established
	//	extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket.
	//	original server_socket is unaffected and new socket is not in listening state
	//	we are leaving it empty for now, because we're connecting with our own client locally
	//	you can pass the address of a struct that will give you information on where the client is connecting from
	//	if the socket is marked nonblocking and no pending connections are present on the queue, accept() fails with the error EAGAIN or EWOULDBLOCK.
	//	in order to be notified of incoming connections on a socket, you can use select(2), poll(2), or epoll(7).  A readable event will be delivered when a new connection is attempted and you may then call accept() to get a socket for that connection.
	int	client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == -1)
	{
		return (error_and_exit("An error occured in accept.\n"));
	}

	// //	send
	// //	ssize_t send(int sockfd, const void *buf, size_t len, int flags);
	// //	without flags send is equal to write
	// //	only works when the socket is in a connected state
	int	number_of_bytes_send;
	number_of_bytes_send = send(client_socket, &server_message[0], ft_strlen(server_message), 0);
	// -1
	
	// //	close
	// //	close destroys the socket
	int	close_status;
	close_status = close(server_socket);
	if (close_status == -1)
	{
		error_and_exit("An error occured in closing the server socket.\n");
	}
	// close_status = close(client_socket);
	// if (close_status == -1)
	// {
	// 	error_and_exit("An error occured in closing the client socket.\n");
	// }
	return (0);
}
