/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   virtual_server.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 14:45:03 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/15 16:05:17 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef VIRTUAL_SERVER_HPP
# define VIRTUAL_SERVER_HPP

#include "../config_parser/server.hpp"
#include "./select_location.hpp"
#include "./select_virtual_server.hpp"

class VirtualServer {
	public:
		VirtualServer(std::string const& host, std::string const& uri_path, std::vector<Server> servers)
		~VirtualServer();
		VirtualServer(VirtualServer const& other);
		VirtualServer&	operator=(VirtualServer const& other);
		
		void	overwrite_directives_if_set_in_location(LocationBlock location_block);
		
		std::string const &												get_autoindex() const;
		std::string const &												get_client_max_body_size() const;
		std::vector<std::pair<std::vector<int>, std::string> > const &	get_error_page() const;
		std::vector<std::string> const &								get_index() const;
		std::vector<std::string> const &								get_limit_except() const;
		std::vector<std::string> const &								get_listen() const;
		std::string const &												get_root() const;
		std::string const &												get_server_name() const;
	private:
		VirtualServer() {}
		std::string												_autoindex;
		std::string												_client_max_body_size;
		std::vector<std::pair<std::vector<int>, std::string> >	_error_page;
		std::vector<std::string>								_index;
		std::vector<std::string>								_limit_except;
		std::vector<std::string>								_listen;
		std::string												_root;
		std::string												_server_name;
};

#endif
