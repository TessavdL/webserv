/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:27:10 by jelvan-d      #+#    #+#                 */
/*   Updated: 2023/01/11 17:31:34 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "./server_config.hpp"
# include "./location_block.hpp"
# include "../http_request/utility_split.hpp"

class Server : public ServerConfig {
	private:
		int									_seen_server_name;
		int									_seen_listen;
		int									_seen_root;
		int									_seen_client_max_body_size;
		int									_seen_index;
		int									_seen_autoindex;
		int									_seen_return;
		int									_seen_rewrite;
		vector<string>						_server_name;
		vector<string>						_listen;
		vector<pair<string, int> >			_host_and_port;
		vector<LocationBlock>				_location_blocks;
	public:
		Server();
		Server(ConfigLexer::t_server server);
		Server(Server const& other);
		Server &operator=(Server const& rhs);
		virtual ~Server();
		void	get_directives(ConfigLexer::t_server server);
		void	error_check_listen(vector<string> const& listen);
		void	initialise_seen_directives(void);
		void	check_duplicate_directives(void);
		vector<string> const&				get_server_name() const;
		vector<string> const&				get_listen() const;
		vector<pair<string, int> > const&	get_host_and_port() const;
		vector<LocationBlock> const&		get_location_block() const;
};

std::ostream&	operator<<(std::ostream& os, Server const& server_block);

#endif
