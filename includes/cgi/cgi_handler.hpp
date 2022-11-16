/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi_handler.hpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/16 16:40:01 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/16 17:28:07 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HANDLER_HPP
# define CGI_HANDLER_HPP

# include "./cgi.hpp"
# include "../event_loop/connection.hpp"

void	create_cgi_response(Connection& connection, std::string const& file_location);

#endif
