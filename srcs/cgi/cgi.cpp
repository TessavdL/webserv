/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:28 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/16 14:47:59 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi.hpp"

static int	get_port_number_from_socket_fd(int const& connection_fd) {
	struct sockaddr_in	local_sin;
	socklen_t			local_sin_len = sizeof(local_sin);
	
	if (getsockname(connection_fd, (struct sockaddr *)&local_sin, &local_sin_len) != -1)
		throw(FatalException("SYSCALL: getsockname: Failed\n"));
	return (ntohs(local_sin.sin_port));
}

Cgi::Cgi(void) {
	return ;
}

Cgi::Cgi(Connection const&	connection, int const& connection_fd) {
	create_argv();
	create_env(connection, connection.get_request(), connection_fd);
	initiate_cgi_process();
}

Cgi::Cgi(Cgi const& other) {
	*this = other;
}

Cgi&	Cgi::operator=(Cgi const& rhs) {
	if (this != &rhs)
		return (*this);
	return (*this);
}

Cgi::~Cgi(void) {
	free_env_array();
	return ;
}

void	Cgi::create_argv(void) {
	this->_argv[2] = NULL;
	this->_argv[0] = strdup("/usr/bin/php");
	this->_argv[1] = NULL;
}

void	Cgi::create_env(Connection const& connection, Connection::t_request const& request, int const& connection_fd) {
	string	cwd = string(getcwd(NULL, 0));

	if (request.request_line.method.compare("POST"))
		this->_env["CONTENT_LENGTH"] = request.bytes_in_data;
	else
		this->_env["CONTENT_LENGTH"] = to_string(request.request_line.uri.get_query_string().size());
	if (request.request_line.method.compare("GET"))
		this->_env["CONTENT_TYPE"] = "text/html";
	else
		this->_env["CONTENT_TYPE"] = request.headers.find("Content-Type")->second;
	this->_env["PATH_INFO"] = request.request_line.uri.get_path_full();
	this->_env["PATH_TRANSLATED"] = cwd + '/' + request.request_line.uri.get_path_full();
	this->_env["QUERY_STRING"] = request.request_line.uri.get_query_string();
	this->_env["REMOTE_HOST"] = request.request_line.uri.get_authority_host();
	this->_env["REQUEST_METHOD"] = request.request_line.method;
	this->_env["SCRIPT_FILENAME"] = request.request_line.uri.get_path_full();
	this->_env["SCRIPT_NAME"] = request.request_line.uri.get_path_full();
	this->_env["SERVER_NAME"] = request.headers.find("Host")->second;
	this->_env["SERVER_PORT"] = to_string(get_port_number_from_socket_fd(connection_fd));
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Codyserv (macOS)";
	create_env_from_map();
}

void	Cgi::create_env_from_map(void) {
	this->_env_array = malloc(sizeof(char *) * this->_env.size() + 1);
	if (!this->_env_array)
		throw(FatalException("SYSCALL: malloc: Failed\n"));
	for (map<string, string>::iterator it = this->_env.begin(); it != this->_env.end(); it++)
		this->_env_array[]
}

void	Cgi::free_env_array(void) {
	for (int i = 0; this->_env_array[i]; ++i)
		free(this->_env_array[i]);
	free(this->_env_array);
}

void	Cgi::initiate_cgi_process(void) {
	this->_pid = fork();
	if (this->_pid == -1)
		throw(FatalException("SYSCALL: fork: Failed\n"));
	if (this->_pid == 0)
		execve(this->_argv[0], this->_argv, this->_env);
}
