/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/18 15:19:20 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/10/19 17:41:08 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>

#include "../../includes/http_response/response_utils.hpp"

class Response {
	public:
		Response();
		~Response();
		Response(Response const& other);
		Response&	operator=(Response const& other);
		void				set_status_line();
		void				set_headers();
		void				set_body(void);
		void				set_full_response();
		std::string const&	get_full_response() const;
		std::string const&	get_status_line() const;
		std::string const&	get_headers() const;
		std::string const&	get_body() const;
	private:
		std::string		_full_response;
		std::string		_status_line;
		std::string		_headers;
		std::string		_body;
};

std::ostream&	operator<<(std::ostream& os, Response const& response);

#endif
