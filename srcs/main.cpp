/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 14:38:25 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/10 16:24:16 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"

int	main(int ac, char **av) {
	string filename;
	
	if (ac == 1)
		filename = "./configs/good_configs/basic.conf";
	else if (ac == 2)
		filename = string(av[1]);
	else {
		cerr << "Error, please provide a valid amount of arguments" << endl;
		exit(1);
	}
	try {
		vector<Server>	server_blocks;
		ConfigLexer		server_config(filename);
		// Prints the raw server content before parsing
		// cout << server_config << endl;
		vector<ConfigLexer::t_server>	serv = server_config.get_server_blocks();
		for (vector<ConfigLexer::t_server>::iterator it = serv.begin(); it != serv.end(); ++it) {
			server_blocks.push_back(*it);
			// Prints the parsed server content
			// cout << server_blocks.back() << endl;
		}
		event_loop(server_blocks);
	}
	catch (FatalException const& e) {
		cerr << e.what() << endl;
		exit(1);
	}
	catch (ConfigException const& e) {
		cerr << e.what() << endl;
		exit(1);
	}
	return (0);
}
