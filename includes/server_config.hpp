/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_config.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/09/14 14:42:06 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/09/14 18:11:30 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONFIG_HPP
# define SERVER_CONFIG_HPP

# include <string>
# include <vector>

class location;
using namespace std;

class server_config
{
private:
		string						_root;
		string						_client_max_body_size;
		string						_limit_except;
		vector<string>				_server_names;
		vector<string>				_index;
		vector<pair<int, string>>	_error_pages;
		vector<pair<string, int>>	_listen;
		bool						_autoindex;
	protected:

	public:
		server_config();
		server_config(server_config const& other);
		server_config &operator=(server_config const& rhs);
		virtual ~server_config();
};

#endif
