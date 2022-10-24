/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 16:27:10 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/18 16:38:48 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

# include "./server_config.hpp"
# include "./location_block.hpp"
# include "../http_request_parser/utility_split.hpp"
# include "../listening_sockets/socket_listen.hpp"

class Server : public ServerConfig {
	private:
		vector<string>						_server_name;
		vector<string>						_listen;
		vector<pair<string, int> >			_host_and_port;
		vector<LocationBlock>				_location_blocks;
		vector<SocketListen>				_sockets;
	public:
		Server();
		Server(Lexer::t_server server);
		Server(Server const& other);
		Server &operator=(Server const& rhs);
		virtual ~Server();
		void	get_directives(Lexer::t_server server);
		void	error_check_listen(vector<string> const& listen);
		void	set_listening_sockets();
		vector<string> const&			get_server_name() const;
		vector<string> const&			get_listen() const;
		vector<LocationBlock> const&	get_location_block() const;
};

std::ostream&	operator<<(std::ostream& os, Server const& server_block);

#endif
