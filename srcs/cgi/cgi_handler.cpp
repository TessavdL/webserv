/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi_handler.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 16:39:39 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/16 17:23:27 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cgi/cgi_handler.hpp"

void	create_cgi_response(Connection& connection, std::string const& file_location) {
	Cgi	cgi_session(connection, file_location);
}
