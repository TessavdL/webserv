/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   select_location.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/09 14:26:01 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/14 16:59:59 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SELECT_LOCATION_HPP
# define SELECT_LOCATION_HPP

#include <string>
#include <vector>

#include "../../includes/config_parser/server.hpp"

#define NO_LOCATION -2

size_t	determine_location_index(std::string const& host, std::vector<LocationBlock> const& v);

#endif
