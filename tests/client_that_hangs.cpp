#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#include <string>
#include <vector>
#include <iostream>

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		return (1);
	}
	int	client_socket;
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << client_socket << "\n";
	if (client_socket == -1)
	{
		return (error_and_exit("An error occured in socket.\n"));
	}

	int	n = 1;
	int status = setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &n, sizeof(n));

	struct sockaddr_in	client_socket_address;
	client_socket_address.sin_family = AF_INET;
	client_socket_address.sin_port = htons(8080);
	client_socket_address.sin_addr.s_addr = INADDR_ANY;
	
	int connection_status;
	connection_status = connect(client_socket, (struct sockaddr *)&client_socket_address, sizeof(client_socket_address));
	if (connection_status == -1)
	{
		return (error_and_exit("An error occured in connect.\n"));
	}

	sleep(atoi(argv[1]));

	int	close_status;
	close_status = close(client_socket);
	if (close_status == -1)
	{
		error_and_exit("An error occured in close.\n");
	}
	std::cout << "Client fd was closed\n";
	return (0);
}
