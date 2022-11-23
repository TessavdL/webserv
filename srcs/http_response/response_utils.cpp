/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response_utils.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 18:11:22 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/23 12:50:41 by tevan-de      ########   odam.nl         */
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

bool	check_if_auto_index_is_on(std::string const& auto_index) {
	if (!auto_index.compare("on")) {
		return (true);
	}
	return (false);
}

std::string create_directory_list_page(std::string const& file) {
	std::vector<std::string>	v = get_directory_file_list(file);
	std::string					directories;
	std::string					page = DIRECTORY_LISTING_STRING;

	for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); it++) {
		directories = directories + "<ul><a href = \"" + *it + "/\"/><p>" + *it + "</p></a></ul>";
	}
	page.replace(page.find("$FILE"), strlen("$FILE"), file);
	page.replace(page.find("$FILE"), strlen("$FILE"), file);
	page.replace(page.find("$DIRECTORIES"), strlen("$DIRECTORIES"), directories);
	return (page);
}

bool	isCGI(std::string const& file_location) {
	std::string const& extension = file_location.substr(file_location.find("."));

	if (!extension.empty() && !extension.compare(".php"))
		return (true);
	return (false);
}
