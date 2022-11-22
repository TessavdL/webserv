/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_page_string.hpp                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/22 14:05:05 by tevan-de      #+#    #+#                 */
/*   Updated: 2022/11/22 16:00:51 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_PAGE_STRING_HPP
# define ERROR_PAGE_STRING_HPP

#define ERROR_PAGE_STRING "<!DOCTYPE html>\
<html>\
	<head>\
		<meta charset=\"utf-8\">\
		<title>$STATUS_CODE $REASON_PHRASE</title>\
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
			.error-container {\
				text-align: center;\
				position: fixed;\
  				top: 50%;\
  				left: 50%;\
  				transform: translate(-50%, -50%);\
			}\
			.error-container h1 {\
				margin: 0;\
				font-size: 130px;\
				font-weight: 300;\
			}\
			.error-container p {\
				margin: 0;\
				font-size: 40px;\
				font-weight: 100;\
			}\
		</style>\
	</head>\
	<body>\
		<div class=\"error-container\">\
			<h1>$STATUS_CODE</h1>\
			<p>$REASON_PHRASE</p>\
		</div>\
	</body>\
</html>"

#endif
