/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 16:22:33 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/08/08 16:45:07 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <stdlib.h>		//for exit
#include <stdio.h>		//for perror
#include <sys/socket.h>

void	error_and_exit(const char* error_message)
{
	perror(error_message);
	exit(EXIT_FAILURE);
}

int	main(void)
{
	// int socket(int domain, int type, int protocol);
	int	socket_fd;

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		error_and_exit("socket failed");

	
	std::cout << "Hello" << std::endl;
	return (EXIT_SUCCESS);
}
