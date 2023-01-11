/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_data.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 13:17:25 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:37:30 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_DATA_HPP
# define RESPONSE_DATA_HPP

# include <map>
# include <string>

class ResponseData
{
	public:
		ResponseData();
		~ResponseData();
		ResponseData(ResponseData const& other);
		ResponseData&	operator=(ResponseData const& other);

		int const&									get_bytes_in_data() const;
		int const&									get_total_bytes_read() const;
		int const&									get_total_bytes_sent() const;
		int const&									get_status_code() const;
		std::string const&							get_reason_phrase() const;
		std::map<std::string, std::string> const&	get_headers() const;
		std::string const&							get_body() const;
		bool const&									get_generated() const;
		std::string const&							get_cgi_response() const;
		std::string const&							get_repsonse_string() const;

		void										set_bytes_in_data(int const bytes_in_data);
		void										set_total_bytes_read(int const total_bytes_read);
		void										set_total_bytes_sent(int const total_bytes_sent);
		void										set_status_code(int const status_code);
		void										set_reason_phrase(std::string const& reason_phrase);
		void										set_headers(std::map<std::string, std::string> const& headers);
		void										set_body(std::string const& body);
		void										set_generated(bool const generated);
		void										set_cgi_response(std::string const& full_response);
		void										set_response_string(std::string const& response_string);
	private:
		int											_bytes_in_data;
		int											_total_bytes_read;
		int											_total_bytes_sent;
		int											_status_code;
		std::string									_reason_phrase;
		std::map<std::string, std::string>			_headers;
		std::string									_body;
		bool										_generated;
		std::string									_cgi_response;
		std::string									_response_string;
};

#endif
