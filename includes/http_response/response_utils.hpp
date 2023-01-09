/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_utils.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:48:46 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 15:48:38 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_UTILS_HPP
# define RESPONSE_UTILS_HPP

# include <ctime>
# include <map>
# include <stdlib.h>
# include <string>
# include <unistd.h>

# include "./directory_listing_string.hpp"
# include "../event_loop/connection.hpp"
# include "../http_request/utility_path.hpp"
# include "../virtual_server/search_for_file.hpp"
# include "../virtual_server/virtual_server.hpp"
# include "../fatal_exception.hpp"

bool						check_if_auto_index_is_on(std::string const& auto_index);
bool						connection_is_continue(std::map<std::string, std::string> const& headers);
bool						is_cgi(std::string const& file_location);
// bool						is_client(std::map<int, Connection> const& connections, int event_fd);
bool						is_return(std::pair<int, std::string> const& virtual_server_return);

char*						custom_asctime(const struct tm* timeptr);
std::string					get_date_information(void);
std::string					create_current_working_directory(void);
std::string					create_directory_list_page(std::string const& file, std::string const& original_uri);

std::map<int, std::string>	create_status_code_reason_phrase_map(void);
std::string const&			get_reason_phrase(int const status_code);

template <typename Iter>
bool	is_last_iterator(Iter current, Iter end) {
	if (std::distance(current, end) == 1)
		return (true);
	return (false);
}

#endif
