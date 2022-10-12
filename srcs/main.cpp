/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 14:38:25 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 13:37:38 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/config_parser/server_block.hpp"
#include "../includes/config_parser/lexer.hpp"
#include "../includes/listening_sockets/socket_listen.hpp"
#include "../includes/fatal_exception.hpp"

int		main(int ac, char **av) {
	if (ac != 2) {
		cout << "Incorrect amount of arguments, please provide a single configuration file." << endl;
		exit (1);
	}
	try {
		vector<ServerBlock>	server_blocks;
		Lexer server_config((string(av[1])));
		// Prints the raw server content before parsing
		// cout << server_config << endl;
		vector<Lexer::t_server>	serv = server_config.get_server_blocks();
		for (vector<Lexer::t_server>::iterator it = serv.begin(); it != serv.end(); ++it) {
			server_blocks.push_back(*it);
			// Prints the parsed server content
			// cout << server_blocks.back() << endl;
		}
	} catch(FatalException const& e) {
		cout << e.what() << endl;
	} catch(LexerParserException const& e) {
		cout << e.what() << endl;
	}
	return (0);
}