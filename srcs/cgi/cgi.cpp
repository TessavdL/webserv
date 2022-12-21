/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:28 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/13 16:55:35 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi.hpp"

static int	get_port_number_from_socket_fd(int const& connection_fd) {
	struct sockaddr_in	local_sin;
	socklen_t			local_sin_len = sizeof(local_sin);
	
	if (getsockname(connection_fd, (struct sockaddr *)&local_sin, &local_sin_len) == -1)
		throw(FatalException("SYSCALL: getsockname: Failed\n"));
	return (ntohs(local_sin.sin_port));
}

Cgi::Cgi(void) {
	return ;
}

Cgi::Cgi(Connection const& connection, std::string const& file_location) {
	create_argv(file_location);
	create_env(connection, connection.get_request(), file_location);
	initiate_cgi_process(connection.get_request());
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
	free(this->_argv[0]);
	if (this->_argv[1])
		free(this->_argv[1]);
	return ;
}

void	Cgi::create_argv(std::string const& file_location) {
	this->_argv[0] = strdup("/Users/jelvan-d/.brew/Cellar/php/8.1.13/bin/php-cgi");
	if (!file_location.empty())
		this->_argv[1] = strdup(file_location.c_str());
	this->_argv[2] = NULL;
}

void	Cgi::create_env(Connection const& connection, RequestData const& request, std::string const& file_location) {
	string	cwd = string(getcwd(NULL, 0));

	if (!request.get_method().compare("POST"))
		this->_env["CONTENT_LENGTH"] = to_string(request.get_body().size());
	else
		this->_env["CONTENT_LENGTH"] = to_string(request.get_uri().get_query_string().size());
	if (!request.get_method().compare("GET"))
		this->_env["CONTENT_TYPE"] = "text/html";
	else
		this->_env["CONTENT_TYPE"] = request.get_headers().find("Content-Type")->second;
	this->_env["PATH_INFO"] = file_location;
	this->_env["PATH_TRANSLATED"] = file_location;
	this->_env["QUERY_STRING"] = request.get_uri().get_query_string();
	this->_env["REMOTE_HOST"] = request.get_uri().get_authority_host();
	this->_env["REQUEST_METHOD"] = request.get_method();
	this->_env["SCRIPT_FILENAME"] = file_location;
	this->_env["SCRIPT_NAME"] = file_location;
	this->_env["SERVER_NAME"] = file_location;
	this->_env["SERVER_PORT"] = to_string(get_port_number_from_socket_fd(connection.get_connection_fd()));
	this->_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	this->_env["SERVER_SOFTWARE"] = "Codyserv (macOS)";
	// // for (map<string, string>::iterator it = this->_env.begin(); it != this->_env.end(); ++it)
	// // 	cout << it->first << "=" << it->second << endl;
	create_env_from_map();
	(void)connection;
}

void	Cgi::create_env_from_map(void) {
	size_t	i(0);

	this->_env_array = (char **)malloc(sizeof(char *) * (this->_env.size() + 1));
	if (!this->_env_array)
		throw(FatalException("SYSCALL: malloc: Failed\n"));
	for (map<string, string>::iterator it = this->_env.begin(); it != this->_env.end(); it++) {
		this->_env_array[i] = strdup(string(it->first + "=" + it->second).c_str());
		i++;
	}
	this->_env_array[i] = NULL;
}

void	Cgi::free_env_array(void) {
	for (int i = 0; this->_env_array[i]; i++)
		free(this->_env_array[i]);
	free(this->_env_array);
}

void	Cgi::initiate_cgi_process(RequestData const& request) {
	if (pipe(this->_fd[0]) < 0 || pipe(this->_fd[1]) < 0)
		throw(FatalException("SYSCALL: pipe: Failed\n"));
	if (request.get_method().compare("POST")) {
		close(this->_fd[1][0]);
		close(this->_fd[1][1]);
	}
	this->_pid = fork();
	if (this->_pid == -1)
		throw(FatalException("SYSCALL: fork: Failed\n"));
	if (this->_pid == 0)
		child_process(request);
	parent_process(request);
}

void	Cgi::child_process(RequestData const& request) {

	if (!request.get_method().compare("POST")) {
		if (dup2(this->_fd[1][0], STDIN_FILENO) == -1)
			throw (FatalException("SYSCALL: dup2: Failed\n"));
	}
	if (this->_fd[0][1] != STDOUT_FILENO) {
		if (dup2(this->_fd[0][1], STDOUT_FILENO) == -1)
			throw (FatalException("SYSCALL: dup2: Failed\n"));
	}
	if (!request.get_method().compare("POST"))
		close(this->_fd[1][1]);
	close(this->_fd[0][0]);
	if (execve(this->_argv[0], this->_argv, _env_array) == -1)
		throw (FatalException("SYSCALL: execve: Failed\n"));
}

void	Cgi::parent_process(RequestData const& request) {
	int	exit_status(0);
	int	ret(0);

	if (!request.get_method().compare("POST"))
		close(this->_fd[1][0]);
	close(this->_fd[0][1]);
	if (!request.get_method().compare("POST"))
		write(this->_fd[1][1], request.get_body().c_str(), request.get_body().size());
	if (waitpid(this->_pid, &exit_status, WNOHANG) == -1)
		throw (FatalException("SYSCALL: waitpid: Failed"));
	if (WIFEXITED(exit_status))
		ret = WEXITSTATUS(exit_status);
	get_content_from_cgi();
	close(this->_fd[0][0]);
}

void	Cgi::get_content_from_cgi(void) {
	string tmp;
	int	ret(0);

	this->_body = "\0";
	tmp.resize(PIPE_BUF);
	while ((ret = read(this->_fd[0][0], &tmp[0], PIPE_BUF)) > 0) {
		if (ret == 0 || ret != PIPE_BUF)
			break ;
		tmp.resize(ret);
		this->_body += tmp;
	}
	tmp.resize(ret);
	this->_body += tmp;
	return ;
}

string const&	Cgi::get_body(void) const {
	return (this->_body);
}
