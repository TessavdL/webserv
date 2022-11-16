/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_utils.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 18:11:22 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/16 13:52:00 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/http_response/response_utils.hpp"

char*	custom_asctime(const struct tm* time_information)
{
	const char wday_name[][4] = {
		"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
	};
	const char mon_name[][4] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	static char	date_information[31];

	sprintf(date_information, "%.3s,%3d %.3s %d %.2d:%.2d:%.2d CET",
    	wday_name[time_information->tm_wday],
		time_information->tm_mday,
    	mon_name[time_information->tm_mon],
		1900 + time_information->tm_year,
    	time_information->tm_hour,
    	time_information->tm_min,
		time_information->tm_sec);
  	return (date_information);
}

std::string get_date_information(void) {
 	time_t		rawtime;
	struct tm*	time_information;

	time(&rawtime);
	time_information = localtime(&rawtime);
	return (std::string(custom_asctime(time_information)));
}

std::string	create_current_working_directory(void) {
	char		*buf = NULL;
	char		*ptr = NULL;
	long		size;
	std::string	current_working_directory;

	size = pathconf(".", _PC_PATH_MAX);
	if ((buf = (char *)malloc((size_t)size)) != NULL) {
    	ptr = getcwd(buf, (size_t)size);
	}
	else {
		throw (FatalException("GETCWD"));
	}
	current_working_directory = std::string(buf);
	free(buf);
	return (current_working_directory);
}
