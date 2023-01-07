/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   colors.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: tevan-de <tevan-de@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/01/07 22:25:25 by tevan-de      #+#    #+#                 */
/*   Updated: 2023/01/07 22:26:10 by tevan-de      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
# define COLORS_HPP

#include <ostream>

namespace Color {
		enum Code {
				FG_RED = 31,
				FG_GREEN = 32,
				FG_BLUE = 34,
				FG_DEFAULT = 39,
				BG_RED = 41,
				BG_GREEN = 42,
				BG_BLUE = 44,
				BG_DEFAULT = 49
		};
		class Modifier {
			Code code;
		public:
			Modifier(Code pCode) : code(pCode) {}
			friend std::ostream&
			operator<<(std::ostream& os, const Modifier& mod) {
					return os << "\033[" << mod.code << "m";
			}
		};
}

#endif
