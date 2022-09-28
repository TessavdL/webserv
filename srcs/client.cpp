#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		// close

#include <sys/types.h>
#include <sys/socket.h>	// socket, connect, recv

#include <netinet/in.h> // contains sockaddr_in struct we need for address and port and htons, htonl, ntohl, ntohs

#include <string>
#include <vector>
#include <iostream>

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int main(void)
{
	//	socket
	//	int socket(int domain, int type, int protocol);
	//	domain
	//		AF_INET constant, internetsocket
	//	type
	//		There are three types of sockets: datagram, raw and stream
	//		For TCP protocol we use SOCK_STREAM constant
	//	protocol
	//		unspecified in our example, default = TCP protocol
	int	client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1)
	{
		return (error_and_exit("An error occured in socket.\n"));
	}

	//	socket address
	//	uint16_t	htons(uint16_t hostshort);
	//	htons pass actual port number as parameter
	// 	htons converts our port number to network byte order
	//	INADDR_ANY is same as trying to connect to 0.0.0.0
	struct sockaddr_in	client_socket_address;
	client_socket_address.sin_family = AF_INET;
	client_socket_address.sin_port = htons(9002);
	client_socket_address.sin_addr.s_addr = INADDR_ANY;
	
	//	connect
	//	connect the socket to the address
	//	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	//	first parameter is our network_server we created with socket
	//	second parameter is the network_server address that is cast to const sockaddr, take address
	int connection_status;
	connection_status = connect(client_socket, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
	if (connection_status == -1)
	{
		return (error_and_exit("An error occured in connect.\n"));
	}
	//	https://stackoverflow.com/questions/18670807/sending-and-receiving-stdstring-over-socket
	//	https://www.anycodings.com/1questions/5138429/sending-and-receiving-stdstring-over-socket
	//	receive data
	//	ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	//	first parameter is our network_server we created with socket
	//	second parameter is our buffer
	//	third parameter is our buffer size
	//	the last parameter could be a flag but is 0 in this case
	const unsigned int	max_buf_length = 4096;
	std::vector<char>	buf(max_buf_length);
	std::string			received_message;
	int					bytes_received;

	bytes_received = 1;
	while (bytes_received > 0)
	{
		bytes_received = recv(client_socket, &buf[0], buf.size(), 0);
		if (bytes_received == -1)
		{
			error_and_exit("An error occured in recv.\n");
		}
		if (bytes_received == 0)
		{
			std::cout << "done receiving message" << std::endl;
			break ;
		}
		received_message.append(buf.cbegin(), buf.cend());
	}

	// //	print data
	std::cout << received_message << std::endl;



	//	close socket
	//	close destroys the socket
	int	close_status;
	close_status = close(client_socket);
	if (close_status == -1)
	{
		error_and_exit("An error occured in close.\n");
	}

	return (0);
}
