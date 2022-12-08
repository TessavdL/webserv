/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_data.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 13:25:09 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/08 14:49:16 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_DATA_HPP
# define REQUEST_DATA_HPP

# include <map>
# include <string>

# include "./uri_parser.hpp"

class RequestData {
	public:
		RequestData();
		~RequestData();
		RequestData(RequestData const& other);
		RequestData&	operator=(RequestData const& other);
		std::string const&							get_method() const;
		Uri const&									get_uri() const;
		std::string const&							get_protocol() const;
		std::map<std::string, std::string> const&	get_headers() const;
		std::string const&							get_body() const;
		int const&									get_bytes_in_data() const;
		int const&									get_total_bytes_read() const;
		void										set_bytes_in_data(int const& bytes_in_data);
		void										set_total_bytes_read(int const& total_bytes_read);
		void										set_method(std::string const& method);
		void										set_uri(Uri const& uri);
		void										set_protocol(std::string const& protocol);
		void										set_headers(std::map<std::string, std::string> const& headers);
		void										set_body(std::string const& body);

	private:
		int									_bytes_in_data;
		int									_total_bytes_read;
		std::string							_method;
		Uri									_uri;
		std::string							_protocol;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
};

#endif
