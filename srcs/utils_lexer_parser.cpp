#include "utils_lexer_parser.hpp"

size_t	skip_whitespaces(std::string const& str, size_t index) {
	size_t i = 0;

	for (; str[i + index] && isspace(str[i + index]); i++) {}
	return (i);
}
