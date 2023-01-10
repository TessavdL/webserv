/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_data.hpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 13:25:09 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/10 15:42:09 by jelvan-d      ########   odam.nl         */
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
		int const&									get_total_bytes_read() const;
		void										set_method(std::string const& method);
		void										set_uri(Uri const& uri);
		void										set_protocol(std::string const& protocol);
		void										set_headers(std::map<std::string, std::string> const& headers);
		void										set_body(std::string const& body);
		void										set_total_bytes_read(int const total_bytes_read);

	private:
		std::string							_method;
		Uri									_uri;
		std::string							_protocol;
		std::map<std::string, std::string>	_headers;
		std::string							_body;
		int									_total_bytes_read;
};

#endif
