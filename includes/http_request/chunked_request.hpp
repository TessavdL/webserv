/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   chunked_request.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/07 12:16:51 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/12/07 12:30:04 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHUNKED_REQUEST_HPP
# define CHUNKED_REQUEST_HPP

#include <string>
#include <sstream>

#include "./request_exception.hpp"
#include "./request_handler.hpp"

// #define CRLFLEN 3
// #define CRLF "\\r\n"

std::string chunked_request(std::string const& str, std::string& remainder);

#endif
