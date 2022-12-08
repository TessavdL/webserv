/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_split.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:37:45 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/12/08 12:57:33 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_SPLIT_HPP
# define UTILITY_SPLIT_HPP

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string							string_to_lower(std::string const& str);
std::string							string_until_deilimeter(std::string const& str, std::string const& delimeter);
std::vector<std::string>			split_string_on_delimeter_string(std::string str, std::string const& delimeter);
std::vector<std::string>			split_string_on_delimeter(std::string const& str, char c);
std::pair<std::string, std::string>	split_string_in_half(std::string const& str, std::string delimeter);
std::pair<std::string, std::string>	split_string_in_half_on_any_match(std::string const& str, std::string delimeter);

#endif
