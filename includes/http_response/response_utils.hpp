#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include <ctime>
#include <unistd.h>
#include <stdlib.h>
#include <string>

#include "../fatal_exception.hpp"

char*		custom_asctime(const struct tm* timeptr);
std::string	get_date_information(void);
std::string	create_current_working_directory(void);

template <typename Iter>
bool	is_last_iterator(Iter current, Iter end) {
	if (std::distance(current, end) == 1)
		return (true);
	return (false);
}


#endif
