/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   uri_parser.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/14 16:30:14 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 16:41:10 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef URI_PARSER_HPP
# define URI_PARSER_HPP

# include <map>
# include <string>
# include <vector>
# include <iostream>

# include "./utility_split.hpp"

class Uri {
	public:
		Uri();
		~Uri();
		Uri(Uri const& other);
		Uri&	operator=(Uri const& other);
		void										parse_uri(std::string const& str);
		void										parse_scheme(std::string const& str, size_t& index);
		void										parse_authority(std::string const& str, size_t& index);
		void										parse_path(std::string const& str, size_t& index);
		void										parse_query(std::string const& str, size_t index);
		std::string const&							get_scheme() const;
		std::string const&							get_authority_user_information() const;
		std::string const&							get_authority_host() const;
		int const&									get_authority_port() const;
		std::string const&							get_path_full() const;
		std::string const&							get_path_extension() const;
		std::string const&							get_path_without_extension() const;
		// std::map<std::string, std::string> const&	get_query() const;
		std::string const&							get_query_string() const;
	private:
		std::string									_scheme;
		std::string									_authority_user_information;
		std::string									_authority_host;
		int											_authority_port;
		std::string									_path_full;
		std::string									_path_extension;
		std::string									_path_without_extension;
		// std::map<std::string, std::string>			_query;
		std::string									_query_string;

};

std::ostream&  operator<<(std::ostream& os, Uri const& uri);

#endif
