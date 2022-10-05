/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 14:42:06 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/04 17:44:30 by jelvan-d      ########   odam.nl         */
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

class Location;
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

class Server_Config
{
	private:
		vector<string>						_server_name;
		vector<string>						_listen;
		vector<Location>					_location_blocks;
	protected:
		string								_root;
		string								_client_max_body_size;
		string								_autoindex;
		vector<string>						_index;
		vector<pair<vector<int>, string>>	_error_page;
	public:
		Server_Config();
		Server_Config(t_server server);
		Server_Config(Server_Config const& other);
		Server_Config &operator=(Server_Config const& rhs);
		virtual ~Server_Config();
		void			get_directives(t_server	server);
		directives_list	hash_string(string const& directive);
		int				helper_split(vector<string> &str, string to_split);
		int				helper_split(string &str, string to_split);
		int				helper_split(vector<pair<vector<int>, string>> error_page, string to_split);
};

#endif
