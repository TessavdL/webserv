/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   virtual_server.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/15 14:45:05 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/13 15:58:59 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/virtual_server/virtual_server.hpp"

VirtualServer::VirtualServer(void) {

}

void	VirtualServer::initialize_virtual_server(std::string const& host, std::string const& uri_path, std::vector<Server> servers) {
	size_t const						server_index = select_server_index(host, servers);
	Server const						virtual_server = servers[server_index];
	std::vector<LocationBlock> const	location_blocks = virtual_server.get_location_block();
	int const							location_index = select_location_index(uri_path, location_blocks);
	LocationBlock						location_block;

	this->_autoindex = virtual_server.get_autoindex();
	this->_client_max_body_size = virtual_server.get_client_max_body_size();
	this->_error_page = virtual_server.get_error_page();
	this->_index = virtual_server.get_index();
	this->_listen = virtual_server.get_listen();
	this->_return = virtual_server.get_return();
	this->_rewrite = virtual_server.get_rewrite();
	this->_root = virtual_server.get_root();
	this->_server_name = host;
	this->_limit_except.push_back("GET");
	this->_limit_except.push_back("POST");
	this->_limit_except.push_back("DELETE");

	if (location_index != NO_LOCATION) {
		location_block = location_blocks[location_index];
		overwrite_directives_if_set_in_location(location_block);
	}
}

void	VirtualServer::overwrite_directives_if_set_in_location(LocationBlock location_block) {
	if (!location_block.get_autoindex().empty()) {
		this->_autoindex = location_block.get_autoindex();
	}
	if (!location_block.get_cgi().first.empty()) {
		this->_cgi = location_block.get_cgi();
	}
	if (!location_block.get_client_max_body_size().empty()) {
		this->_client_max_body_size = location_block.get_client_max_body_size();
	}
	if (!location_block.get_error_page().empty()) {
		this->_error_page = location_block.get_error_page();
	}
	if (!location_block.get_index().empty()) {
		this->_index = location_block.get_index();
	}
	if (!location_block.get_limit_except().empty()) {
		this->_limit_except = location_block.get_limit_except();
	}
	if (location_block.get_return().first != 0) {
		this->_return = location_block.get_return();
	}
	if (!location_block.get_rewrite().empty()) {
		this->_rewrite = location_block.get_rewrite();
	}
	if (!location_block.get_root().empty()) {
		this->_root = location_block.get_root();
	}
}

VirtualServer::~VirtualServer(void) {

}

VirtualServer::VirtualServer(VirtualServer const& other) {
	*this = other;
}

VirtualServer&	VirtualServer::operator=(VirtualServer const& other) {
	if (this != &other) {
		this->_autoindex = other._autoindex;
		this->_cgi = other._cgi;
		this->_client_max_body_size = other._client_max_body_size;
		this->_error_page = other._error_page;
		this->_index = other._index;
		this->_limit_except = other._limit_except;
		this->_listen = other._listen;
		this->_return = other._return;
		this->_rewrite = other._rewrite;
		this->_root = other._root;
		this->_server_name =other._server_name;
	}
	return (*this);
}

std::string const &												VirtualServer::get_autoindex(void) const {
	return (this->_autoindex);
}

std::pair<std::string, std::string> const &						VirtualServer::get_cgi(void) const {
	return (this->_cgi);
}

std::string const &												VirtualServer::get_client_max_body_size(void) const {
	return (this->_client_max_body_size);
}

std::vector<std::pair<std::vector<int>, std::string> > const &	VirtualServer::get_error_page(void) const {
	return (this->_error_page);
}

std::vector<std::string> const &								VirtualServer::get_index(void) const {
	return (this->_index);
}

std::vector<std::string> const &								VirtualServer::get_limit_except(void) const {
	return (this->_limit_except);
}

std::vector<std::string> const &								VirtualServer::get_listen(void) const {
	return (this->_listen);
}

std::pair<int, std::string> const&								VirtualServer::get_return(void) const {
	return (this->_return);
}

std::string const &												VirtualServer::get_rewrite(void) const {
	return (this->_rewrite);
}

std::string const &												VirtualServer::get_root(void) const {
	return (this->_root);
}

std::string const &												VirtualServer::get_server_name(void) const {
	return (this->_server_name);
}
