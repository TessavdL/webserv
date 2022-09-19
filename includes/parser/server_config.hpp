/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 14:42:06 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/19 19:18:41 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>
# include <iostream>

class location;
using namespace std;

typedef struct					s_locations
{
	string					path;
	vector<string>	directives;
}								t_locations;
typedef struct					s_server
{
	vector<string>	directives;
	vector<t_locations>	locations;
}								t_server;

enum	directives_list	{
	SERVER_NAME,
	LISTEN,
	ROOT,
	CLIENT_MAX_BODY_SIZE,
	LIMIT_EXCEPT,
	INDEX,
	ERROR_PAGE,
	AUTOINDEX,
	PATH
};

class server_config
{
	private:
		vector<string>				_server_name;
		vector<pair<string, int>>	_listen;
	protected:
		string						_root;
		string						_client_max_body_size;
		string						_limit_except;
		vector<string>				_index;
		vector<pair<int, string>>	_error_page;
		bool						_autoindex;
	public:
		server_config();
		server_config(t_server server);
		server_config(server_config const& other);
		server_config &operator=(server_config const& rhs);
		virtual ~server_config();
		directives_list	hash_string(string const& directive);
		void			helper_split(vector<string> &str, string to_split);
		void			helper_split(string &str, string to_split);
		void			helper_split(vector<pair<string, int>> listen, string to_split);
		void			helper_split(vector<pair<int, string>> error_page, string to_split);
};

#endif
