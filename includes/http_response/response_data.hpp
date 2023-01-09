/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_data.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 13:17:25 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 17:16:36 by tevan-de      ########   odam.nl         */
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
		int const&									get_bytes_in_data(void) const;
		int const&									get_total_bytes_read(void) const;
		void										set_bytes_in_data(int const& bytes_in_data);
		void										set_total_bytes_read(int const& total_bytes_read);
		std::string const&							get_full_response(void) const;
		void										set_body(std::string const& body);
		void										set_full_response(std::string const& full_response);
		void										set_headers(std::map<std::string, std::string> const& headers);
		void										set_reason_phrase(std::string const& reason_phrase);
		void										set_status_code(int const& status_code);
	private:
		int									_bytes_in_data;
		int									_total_bytes_read;
		std::string							_body;
		std::map<std::string, std::string>	_headers;
		std::string							_reason_phrase;
		std::string							_full_response;
		int									_status_code;
};

#endif
