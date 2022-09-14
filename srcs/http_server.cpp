/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   http_server.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 15:26:26 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/09/14 17:39:08 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <string>
#include <iostream>
#include <fstream>

	// Reading from file with cpp
	// const char*		status_line = "HTTP/1.1 200 OK\r\n\n";
	// std::string		response_data;
	// response_data.append(status_line);
	// std::ifstream	input("index.html");
	// for (std::string line; getline(input, line); )
	// {
	// 	response_data.append("\n");
	// 	response_data.append(line);
	// }
	// std::cout << response_data << std::endl;

	// HTTP Response: STATUS LINE, HTTP HEADERS, MESSAGE BODY

int	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int	main(void)
{
	FILE *html_data;
	html_data = fopen("index.html", "r");
	char	response_data[1024];
	fgets(response_data, 1024, html_data);
	char	http_header[2048] = "HTTP/1.1 200 OK\r\n\n";
	strcat(http_header, response_data);

	int	server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		return (error_and_exit("An error occured in socket.\n"));
	}

	int yes=1;
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	struct sockaddr_in	server_socket_address;
	server_socket_address.sin_family = AF_INET;
	server_socket_address.sin_port = htons(9000);
	server_socket_address.sin_addr.s_addr = INADDR_ANY;

	int	bind_status;
	bind_status = bind(server_socket, (struct sockaddr *)&server_socket_address, sizeof(server_socket_address));
	if (bind_status == -1)
	{
		return (error_and_exit("An error occured in bind.\n"));
	}

	int	listen_status;
	listen_status = listen(server_socket, 5);
	if (listen_status == -1)
	{
		return (error_and_exit("An error occured in listen.\n"));
	}

	int	client_socket;
	while (1)
	{
		client_socket = accept(server_socket, NULL, NULL);
		send(client_socket, http_header, sizeof(http_header), 0);
		close(client_socket);
	}
	close(server_socket);
	return (0);
}
