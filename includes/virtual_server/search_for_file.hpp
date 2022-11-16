/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   search_for_file.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 13:27:38 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/16 13:29:20 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEARCH_FOR_FILE_HPP
# define SEARCH_FOR_FILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <dirent.h>


std::string	search_for_file_in_dir(std::vector<std::string>	const& v, std::string const& directory);

#endif
