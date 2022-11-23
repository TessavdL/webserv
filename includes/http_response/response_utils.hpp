#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <string>

#include "../http_request_parser/utility_path.hpp"
#include "../http_response/directory_listing_string.hpp"
#include "../fatal_exception.hpp"

char*		custom_asctime(const struct tm* timeptr);
std::string	get_date_information(void);
std::string	create_current_working_directory(void);

bool		check_if_auto_index_is_on(std::string const& auto_index);
std::string	create_directory_list_page(std::string const& file);

template <typename Iter>
bool	is_last_iterator(Iter current, Iter end) {
	if (std::distance(current, end) == 1)
		return (true);
	return (false);
}

bool	isCGI(std::string const& file_location);

#endif
