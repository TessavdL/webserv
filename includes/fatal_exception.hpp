/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   fatal_exception.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/05 16:44:40 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/10/05 16:44:41 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef FATAL_EXCEPTION_HPP
# define FATAL_EXCEPTION_HPP

# include <string>
# include <stdexcept>
# include <errno.h>

class FatalException: public std::exception {
	public:
		FatalException();
		FatalException(std::string const error_information);
		FatalException(FatalException const& src);
		~FatalException() throw();
		FatalException& operator=(FatalException const& other);
		const char *what() const throw();
	private:
		std::string	_message;
};

#endif
