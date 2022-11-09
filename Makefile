# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tevan-de <tevan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/08 16:12:16 by tevan-de      #+#    #+#                  #
#    Updated: 2022/11/09 14:47:38 by tevan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=		webserv

CC			=		g++

CFLAGS		=		-Wall -Wextra -Werror -std=c++98 -pedantic

SRC_DIR		=		srcs

SRC_FILES	=		fatal_exception.cpp \
					kqueue_server.cpp \
					main.cpp \
					event_loop/connection.cpp \
					config_parser/exception_config_parser.cpp \
					config_parser/lexer.cpp \
					config_parser/lexer_utils.cpp \
					config_parser/location_block.cpp \
					config_parser/server.cpp \
					config_parser/server_config.cpp \
					listening_sockets/socket_listen.cpp \
					http_request_parser/exception_http_request_parser.cpp \
					http_request_parser/http_request_lexer.cpp \
					http_request_parser/http_request_parser.cpp \
					http_request_parser/utility_split.cpp \
					http_response/response.cpp \
					http_response/response_utils.cpp \
					virtual_server/select_location.cpp \
					virtual_server/select_virtual_server.cpp

SRCS		=		$(addprefix ${SRC_DIR}/, ${SRC_FILES})

OBJS		=		${SRCS:.cpp=.o}

INCLUDES	=		./includes/

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -g -fsanitize=address

%.o: %.cpp
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
