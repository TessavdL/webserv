/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request_exception.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/23 13:45:28 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/11 17:32:40 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_EXCEPTION_HPP
# define REQUEST_EXCEPTION_HPP

# include <string>
# include <stdexcept>

class RequestException: public std::exception {
	public:
		RequestException(int const& status_code, std::string const& function_name);
		RequestException(RequestException const& src);
		~RequestException(void) throw();
		RequestException& operator=(RequestException const& other);
		char const *what() const throw();
		int const&	get_status_code() const;
	private:
		RequestException();
		int			_status_code;
		std::string	_message;
};

#endif
