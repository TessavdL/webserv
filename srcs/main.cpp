/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 14:38:25 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/21 13:24:14 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/config_parser/server.hpp"
#include "../includes/config_parser/lexer.hpp"
#include "../includes/listening_sockets/socket_listen.hpp"
#include "../includes/fatal_exception.hpp"
#include "../includes/webserv.hpp"

int		main(int ac, char **av) {
	string filename;
	
	if (ac == 1)
		filename = "./basic.conf";
	else if (ac == 2)
		filename = string(av[1]);
	else {
		cout << "Error, please provide a valid amount of arguments" << endl;
		exit (1);
	}
	try {
		vector<Server>		server_blocks;
		Lexer server_config(filename);
		// Prints the raw server content before parsing
		// cout << server_config << endl;
		vector<Lexer::t_server>	serv = server_config.get_server_blocks();
		for (vector<Lexer::t_server>::iterator it = serv.begin(); it != serv.end(); ++it) {
			server_blocks.push_back(*it);
			// Prints the parsed server content
			// cout << server_blocks.back() << endl;
		}
		kqueue_server(server_blocks);

	} catch(FatalException const& e) {
		cout << e.what() << endl;
	} catch(LexerParserException const& e) {
		cout << e.what() << endl;
	}
	return (0);
}
