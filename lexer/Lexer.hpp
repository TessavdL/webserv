#ifndef Lexer_HPP
# define Lexer_HPP

#include <vector>
#include <string>

class Lexer {
	public:
		typedef struct					s_locations
		{
			std::string					path;
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
		std::string	get_raw_data(std::string const& file_name);
		bool		check_brackets(std::string const& raw_data);
		size_t		count_server_blocks(std::string str);
		void		add_directives(std::string str, t_server server_block);
		void		get_server_block(std::string str);
		void		remove_brackets(std::string& str);
		int			even_or_uneven(std::string str);
	private:
		std::vector<t_server>			_server_blocks;
};

#endif
