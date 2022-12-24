/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_utils.hpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:48:46 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/24 12:50:01 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include <ctime>
#include <map>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include "../http_request/utility_path.hpp"
#include "../http_response/directory_listing_string.hpp"
#include "../fatal_exception.hpp"

char*						custom_asctime(const struct tm* timeptr);
std::string					get_date_information(void);
std::string					create_current_working_directory(void);
bool						check_if_auto_index_is_on(std::string const& auto_index);
std::string					create_directory_list_page(std::string const& file);
bool						is_cgi(std::string const& file_location);
std::map<int, std::string>	create_status_code_reason_phrase_map(void);
std::string const&			get_reason_phrase(int const status_code);

template <typename Iter>
bool	is_last_iterator(Iter current, Iter end) {
	if (std::distance(current, end) == 1)
		return (true);
	return (false);
}

#endif
