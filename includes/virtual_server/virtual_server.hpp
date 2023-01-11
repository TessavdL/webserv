/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   virtual_server.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 14:45:03 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:41:32 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

# include "../config_parser/server.hpp"
# include "./select_location.hpp"
# include "./select_virtual_server.hpp"

class VirtualServer {
	public:
		VirtualServer();
		~VirtualServer();
		VirtualServer(VirtualServer const& other);
		VirtualServer&	operator=(VirtualServer const& other);
		
		void	initialize_virtual_server(std::string const& host, std::string const& uri_path, std::vector<Server> servers);
		void	overwrite_directives_if_set_in_location(LocationBlock location_block);
		
		std::string const &												get_autoindex() const;
		std::pair<std::string, std::string> const &						get_cgi() const;
		int		 const &												get_client_max_body_size() const;
		std::vector<std::pair<std::vector<int>, std::string> > const &	get_error_page() const;
		std::vector<std::string> const &								get_index() const;
		std::vector<std::string> const &								get_limit_except() const;
		std::vector<std::string> const &								get_listen() const;
		std::pair<int, std::string> const &								get_return() const;
		std::string const &												get_rewrite() const;
		std::string const &												get_root() const;
		std::string const &												get_server_name() const;
	
	private:
		std::string												_autoindex;
		std::pair<std::string, std::string>						_cgi;
		int														_client_max_body_size;
		std::vector<std::pair<std::vector<int>, std::string> >	_error_page;
		std::vector<std::string>								_index;
		std::vector<std::string>								_limit_except;
		std::vector<std::string>								_listen;
		std::pair<int, std::string>								_return;
		std::string												_rewrite;
		std::string												_root;
		std::string												_server_name;
};

#endif
