/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   directory_listing_string.hpp                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 14:05:05 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/22 18:49:25 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTORY_LISTING_STRING_HPP
# define DIRECTORY_LISTING_STRING_HPP

#define DIRECTORY_LISTING_STRING "<!DOCTYPE html>\
<html>\
	<head>\
		<meta charset=\"utf-8\">\
		<title>Index of $FILE</title>\
		<style>\
			html,\
			body {\
				margin: 0;\
				padding: 0;\
				height: 100vh;\
			}\
			body {\
				font-family: Helvetica, Arial, sans-serif;\
				background-color: #2D72D9;\
				color: white;\
			}\
			h1 {\
				margin: 0;\
				font-size: 50;\
				font-weight: 300;\
			}\
			a p {\
				margin: 0;\
				font-size: 25px;\
				font-weight: 100;\
				color: white;\
			}\
			a::vistited p {\
				color: black;\
			}\
		</style>\
	</head>\
	<body>\
			<h1>Index of $FILE</h1>\
			<ul>\
				$DIRECTORIES\
			</ul>\
		</div>\
	</body>\
</html>"

#endif
