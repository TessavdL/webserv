#ifndef OLD_LEXER_HPP
# define OLD_LEXER_HPP

# include "LexerUtils.hpp"

# include <vector>
# include <string>

class Lexer {
	public:
		typedef struct					s_locations
		{
			std::string					path_and_optional_modifier;
			std::vector<std::string>	directives;
		}								t_locations;
		typedef struct					s_server
		{
			std::vector<std::string>	directives;
			std::vector<t_locations>	locations;
		}								t_server;
		Lexer();
		~Lexer();
		Lexer(Lexer const& other);
		Lexer&		operator=(Lexer const& other);
		std::string	get_input_from_config_file(std::string const& file_name) const;
		// bool		check_brackets(std::string const& raw_data);
		size_t		count_server_blocks(std::string str) const;
		void		add_directives(std::string str, std::vector<std::string> block);
		void		check_brackets(std::string str);
		void		remove_brackets(std::string& str);
		int			even_or_uneven(std::string str);
		std::string		get_block(std::string str, t_server& block);
		std::vector<t_server>	get_private_var(void) const {
			return (this->_server_blocks);
		}
	private:
		std::vector<t_server>			_server_blocks;
};

#endif
