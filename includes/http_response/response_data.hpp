/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_data.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 13:17:25 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/07 12:56:35 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_DATA_HPP
# define RESPONSE_DATA_HPP

#include <map>
#include <string>

class ResponseData
{
	public:
		ResponseData();
		~ResponseData();
		ResponseData(ResponseData const& other);
		ResponseData&	operator=(ResponseData const& other);
		int const&									get_status_code(void) const;
		std::string const&							get_reason_phrase(void) const;
		std::map<std::string, std::string> const&	get_headers(void) const;
		std::string const&							get_body(void) const;
		std::string const&							get_full_response(void) const;
		void										set_body(std::string const& body);
		void										set_full_response(std::string const& full_response);
		void										set_headers(std::map<std::string, std::string> const& headers);
		void										set_reason_phrase(std::string const& reason_phrase);
		void										set_status_code(int const& status_code);	
	private:
		std::string							_body;
		std::map<std::string, std::string>	_headers;
		std::string							_reason_phrase;
		std::string							_full_response;
		int									_status_code;
};

#endif
