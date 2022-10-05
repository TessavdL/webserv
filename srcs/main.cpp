/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 14:38:25 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 17:25:53 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/config_parser/server_config.hpp"
#include "../includes/config_parser/lexer.hpp"
#include "../includes/listening_sockets/socket_listen.hpp"
#include "../includes/fatal_exception.hpp"

int		main(int ac, char **av) {
	if (ac != 2) {
		cout << "Incorrect amount of arguments, please provide a single configuration file." << endl;
		cout << av[1] << endl;
		exit (1);
	}
	return (0);
}
