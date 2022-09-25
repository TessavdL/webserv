#ifndef LEXER_PARSER_EXCEPTION_HPP
# define LEXER_PARSER_EXCEPTION_HPP

# include <string>
# include <stdexcept>

class LexerParserException: public std::exception {
	public:
		LexerParserException(void);
		LexerParserException(std::string const message);
		LexerParserException(LexerParserException const& src);
		~LexerParserException(void) throw();
		LexerParserException& operator=(LexerParserException const& other);
		const char *what(void) const throw();
	private:
		std::string	_message;
};

#endif
