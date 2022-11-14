/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cgi.hpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jelvan-d <jelvan-d@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/01 17:57:29 by jelvan-d      #+#    #+#                 */
/*   Updated: 2022/11/01 18:00:36 by jelvan-d      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

class Cgi {
	public:
		Cgi();
		Cgi(Cgi const& other);
		Cgi& operator=(Cgi const& rhs);
		~Cgi();

	private:
};

#endif
