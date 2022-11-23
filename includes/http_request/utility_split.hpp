/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utility_split.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/12 17:37:45 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/02 12:54:41 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_SPLIT_HPP
# define UTILITY_SPLIT_HPP

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::string	                        string_until_deilimeter(std::string const& str, std::string const& delimeter);
std::vector<std::string>	        split_string_on_delimeter_string(std::string str, std::string const& delimeter);
std::vector<std::string>			split_string_on_delimeter(std::string const& str, char c);
std::pair<std::string, std::string>	split_string_in_half(std::string const& str, std::string delimeter);
std::pair<std::string, std::string>	split_string_in_half_on_any_match(std::string const& str, std::string delimeter);

#endif
