/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exception_http_request_parser.hpp                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:38:17 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/12 17:38:20 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_REQUEST_EXCEPTION_HPP
# define HTTP_REQUEST_EXCEPTION_HPP

# include <string>
# include <stdexcept>

class HTTPRequestException: public std::exception {
	public:
		HTTPRequestException(void);
		HTTPRequestException(std::string const message);
		HTTPRequestException(HTTPRequestException const& src);
		~HTTPRequestException(void) throw();
		HTTPRequestException& operator=(HTTPRequestException const& other);
		const char *what(void) const throw();
	private:
		std::string	_message;
};

#endif
