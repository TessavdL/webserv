#ifndef RESPONSE_UTILS_HPP
#define RESPONSE_UTILS_HPP

#include <string>
#include <ctime>

char*		custom_asctime(const struct tm* timeptr);
std::string	get_date_information(void);
#endif
