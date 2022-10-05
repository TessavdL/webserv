# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tevan-de <tevan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/08 16:12:16 by tevan-de      #+#    #+#                  #
#    Updated: 2022/10/05 15:14:15 by jelvan-d      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=		webserv

CC			=		c++

CFLAGS		=		-Wall -Wextra -Werror -std=c++98 -pedantic

SRC_DIR		=		srcs

SRC_FILES	=		main.cpp \
					FatalException.cpp \
					kqueue_server.cpp \
					config_parser/Exception.cpp \
					config_parser/Lexer.cpp \
					config_parser/LexerUtils.cpp \
					config_parser/Location.cpp \
					config_parser/Server_Config.cpp \
					listening_sockets/SocketListen.cpp

SRCS		=		$(addprefix ${SRC_DIR}/, ${SRC_FILES})

OBJS		=		${SRCS:.cpp=.o}

INCLUDES	=		./includes/

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
