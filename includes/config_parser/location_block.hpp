	/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   location_block.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 17:18:05 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/21 12:01:19 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_BLOCK_HPP
# define LOCATION_BLOCK_HPP

# include "./config_lexer.hpp"
# include "./server_config.hpp"

using namespace std;

class LocationBlock : public ServerConfig
{
	private:
		int						_seen_root;
		int						_seen_client_max_body_size;
		int						_seen_limit_except;
		int						_seen_index;
		int						_seen_autoindex;
		int						_seen_cgi;
		int						_seen_return;
		int						_seen_rewrite;
		pair<string, string>	_cgi;
		vector<string>			_limit_except;
		vector<string>			_path_and_optional_modifier;
	protected:
	public:
		LocationBlock();
		LocationBlock(ConfigLexer::t_locations location);
		LocationBlock(LocationBlock const& other);
		LocationBlock &operator=(LocationBlock const& rhs);
		virtual ~LocationBlock();
		void	get_directives(ConfigLexer::t_locations location);
		void	error_check_limit_except(vector<string> const& limit_except) const;
		void	error_check_path_and_optional_modifier(vector<string> const& path_and_optional_modifier) const;
		void	initialise_seen_directives(void);
		void	check_duplicate_directives(void);
		pair<string, string> const&	get_cgi() const;
		vector<string> const&		get_path_and_optional_modifier() const;
		vector<string> const&		get_limit_except() const;
};

#endif
