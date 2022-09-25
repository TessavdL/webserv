#include "Lexer1.hpp"
#include <iostream>
#include <algorithm>

int	main(int argc, char** argv) {
	Lexer						lexer;
	std::string					input;
	std::vector<std::string>	server_blocks;

	if (argc == 2) {
		input = lexer.get_input_from_config_file(argv[1]);
		server_blocks = lexer.get_server_blocks(input);
		for (std::vector<std::string>::iterator it = server_blocks.begin(); it != server_blocks.end(); it++) {
			std::cout << *it << std::endl;
		}
	}
	return (0);
}
