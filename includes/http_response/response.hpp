/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 14:07:45 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 14:33:46 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_GENERATOR_HPP
# define RESPONSE_GENERATOR_HPP

#include <string>
#include <iostream>

#include "./response_utils.hpp"
#include "./response_data.hpp"

class ResponseGenerator {
	public:
		ResponseGenerator();
		~ResponseGenerator();
		ResponseGenerator(ResponseGenerator const& other);
		ResponseGenerator&	operator=(ResponseGenerator const& other);
		void				generate_response(ResponseData response);
		std::string const&	get_body() const;
		std::string const&	get_full_response() const;
		std::string const&	get_headers() const;
		std::string const&	get_status_line() const;
		void				set_body(std::string const& body);
		void				set_full_response(std::string const& full_response);
		void				set_headers(std::string const& headers);
		void				set_status_line(std::string const& status_line);
	private:
		std::string			_body;
		std::string			_full_response;
		std::string			_headers;
		std::string			_status_line;
};

std::ostream&	operator<<(std::ostream& os, ResponseGenerator const& response);

#endif
