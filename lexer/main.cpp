#include "Lexer.hpp"
#include <iostream>

int	main(int argc, char** argv) {
	if (argc == 2) {
		Lexer	lexer(argv[1]);
		std::cout << lexer;
	}
	return (0);
}
