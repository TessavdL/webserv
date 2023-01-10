/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   prepare_response.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 15:28:04 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 17:33:57 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PREPARE_RESPONSE_HPP
# define PREPARE_RESPONSE_HPP

# include "./response_handler.hpp"
# include "./response_generator.hpp"
# include "./response_utils.hpp"
# include "../event_loop/connection.hpp"
# include "../event_loop/kqueue_and_kevent_utils.hpp"
# include "../event_loop/colors.hpp"

void	prepare_response(Connection& client);

#endif
