/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   send_data.hpp                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/09 14:24:07 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/09 16:26:44 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEND_RESPONSE_HPP
# define SEND_RESPONSE_HPP

// # include "../event_loop/connection.hpp"
# include "../event_loop/kqueue_and_kevent_utils.hpp"
# include "./response_utils.hpp"

// void	send_response_to_client(std::map<int, Connection>& connections, int const kq, struct kevent& event);

class SendData {
	public:
		SendData(std::string const& response_string);
		~SendData();
		SendData(SendData const& other);
		SendData&		operator=(SendData const& other);
		void			add_bytes_sent_to_total_bytes_sent(int const& bytes_sent);
		ssize_t const&	get_total_bytes_sent() const;
		void			set_total_bytes_sent(int const total_bytes_sent);
	private:
		SendData();
		ssize_t		_size;
		ssize_t		_total_bytes_sent;
		std::string	_response_string;
};

#endif
