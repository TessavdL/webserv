#include "Lexer.hpp"
#include <iostream>

int	main(int argc, char** argv) {
	Lexer			lexer;
	std::string		input;
	size_t			server_blocks;

	if (argc == 2) {
		input = lexer.get_input_from_config_file(argv[1]);
		input.erase(std::remove(input.begin(), input.end(), '\n'), input.cend());
		server_blocks = lexer.count_server_blocks(input);
		std::string remainder = input;
		for (size_t i = 0; i < server_blocks; i++) {
			Lexer::t_server	block;
			remainder = lexer.get_block(remainder, block);
			lexer.get_private_var().push_back(block);
		}
	}
	return (0);
}

# TO DO: overwrite ostream so it prints lexer