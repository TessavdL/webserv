/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/06 14:26:14 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/10 13:34:31 by jelvan-d      ########   odam.nl         */
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
	DIRECTIVE_LIST_ERROR = -1
};

class ServerConfig
{
	private:
	protected:
		string								_root;
		string								_client_max_body_size;
		string								_autoindex;
		vector<string>						_index;
		vector<pair<vector<int>, string> >	_error_page;
	public:
		ServerConfig();
		ServerConfig(ServerConfig const& other);
		ServerConfig &operator=(ServerConfig const& rhs);
		virtual ~ServerConfig();
		directives_list	hash_string(string const& directive);
		int				helper_split(vector<string> &str, string to_split);
		int				helper_split(string &str, string to_split);
		int				helper_split(vector<pair<vector<int>, string> > &error_page, string to_split);
		string const&								get_root() const;
		string const&								get_client_max_body_size() const;
		string const&								get_autoindex() const;
		vector<string> const&						get_index() const;
		vector<pair<vector<int>, string> > const&	get_error_page() const;
};

#endif
