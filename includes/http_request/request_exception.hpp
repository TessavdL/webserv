/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_exception.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:45:28 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/23 13:46:18 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_EXCEPTION_HPP
# define REQUEST_EXCEPTION_HPP

# include <string>
# include <stdexcept>

class RequestException: public std::exception {
	public:
		RequestException(void);
		RequestException(std::string const message);
		RequestException(RequestException const& src);
		~RequestException(void) throw();
		RequestException& operator=(RequestException const& other);
		const char *what(void) const throw();
	private:
		std::string	_message;
};

#endif
