# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tevan-de <tevan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/08 16:12:16 by tevan-de      #+#    #+#                  #
#    Updated: 2022/08/08 16:26:25 by tevan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=		webserv

CC			=		c++

CFLAGS		=		-Wall -Wextra -Werror -std=c++98 -pedantic

SRC_DIR		=		srcs

SRC_FILES	=		main.cpp

SRCS		=		$(addprefix ${SRC_DIR}/, ${SRC_FILES})

OBJS		= 		${SRCS:.cpp=.o}

INCLUDES 	=

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
