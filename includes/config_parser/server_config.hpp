/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/06 14:26:14 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/21 11:59:32 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <iostream>
# include <iterator>
# include <sstream>
# include <cstring>
# include <utility>
# include "./lexer.hpp"
# include "./exception_config_parser.hpp"

using namespace std;

enum	directives_list	{
	SERVER_NAME,
	LISTEN,
	ROOT,
	CLIENT_MAX_BODY_SIZE,
	LIMIT_EXCEPT,
	INDEX,
	ERROR_PAGE,
	AUTOINDEX,
	PATH,
	CGI,
	RETURN,
	REWRITE,
	DIRECTIVE_LIST_ERROR = -1
};

class ServerConfig
{
	private:
	protected:
		string								_autoindex;
		string								_client_max_body_size;
		vector<pair<vector<int>, string> >	_error_page;
		vector<string>						_index;
		pair<int, string>					_return;
		string								_rewrite;
		string								_root;
	public:
		ServerConfig();
		ServerConfig(ServerConfig const& other);
		ServerConfig &operator=(ServerConfig const& rhs);
		virtual ~ServerConfig();
		directives_list	hash_string(string const& directive);
		void				helper_split(vector<string> &str, string to_split);
		void				helper_split(string &str, string to_split);
		void				helper_split(pair<string, string> &ret, string to_split);
		void				helper_split(pair<int, string> &ret, string to_split);
		void				helper_split(vector<pair<vector<int>, string> > &error_page, string to_split);
		string const&								get_autoindex() const;
		string const&								get_client_max_body_size() const;
		vector<pair<vector<int>, string> > const&	get_error_page() const;
		vector<string> const&						get_index() const;
		pair<int, string> const&					get_return() const;
		string const&								get_rewrite() const;
		string const&								get_root() const;
};

#endif
