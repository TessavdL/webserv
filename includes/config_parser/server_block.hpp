/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server_block.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 18:45:23 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 20:02:06 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

# include "./server_config.hpp"
# include "./location_block.hpp"
# include "../http_request_parser/utility_split.hpp"

class ServerBlock : public ServerConfig {
	private:
		vector<string>						_server_name;
		vector<string>						_listen;
		vector<pair<string, int> >			_host_and_port;
		vector<LocationBlock>				_location_blocks;
	public:
		ServerBlock();
		ServerBlock(Lexer::t_server server);
		ServerBlock(ServerBlock const& other);
		ServerBlock &operator=(ServerBlock const& rhs);
		virtual ~ServerBlock();
		void	get_directives(Lexer::t_server server);
		void	error_check_listen(vector<string> const& listen);
		vector<string> const&			get_server_name() const;
		vector<string> const&			get_listen() const;
		vector<LocationBlock> const&	get_location_block() const;
};

std::ostream&	operator<<(std::ostream& os, ServerBlock const& server_block);

#endif
