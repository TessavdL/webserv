#include "Lexer.hpp"
#include <iostream>

int	main(int argc, char** argv) {
	Lexer		lexer;
	std::string	raw_data;

	if (argc == 2) {
		raw_data = lexer.get_raw_data(argv[1]);
		std::cout << lexer.count_server_blocks(raw_data) << std::endl;
		// std::cout << raw_data << std::endl;
		lexer.get_server_block(raw_data);
		lexer.check_brackets(raw_data);
	}
	return (0);
}
