# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    Makefile                                           :+:    :+:             #
#                                                      +:+                     #
#    By: tevan-de <tevan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2022/08/08 16:12:16 by tevan-de      #+#    #+#                  #
#    Updated: 2022/12/30 13:49:18 by tevan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

NAME		=		webserv

CC			=		g++

CFLAGS		=		-Wall -Wextra -Werror -std=c++98 -pedantic -g -fsanitize=address

SRC_DIR		=		srcs

SRC_FILES	=		fatal_exception.cpp \
					kqueue_server.cpp \
					kqueue_utils.cpp \
					main.cpp \
					cgi/cgi.cpp \
					cgi/cgi_handler.cpp \
					event_loop/connection.cpp \
					config_parser/exception_config_parser.cpp \
					config_parser/lexer.cpp \
					config_parser/lexer_utils.cpp \
					config_parser/location_block.cpp \
					config_parser/server.cpp \
					config_parser/server_config.cpp \
					listening_sockets/socket_listen.cpp \
					http_request/chunked_request.cpp \
					http_request/request_data.cpp \
					http_request/request_exception.cpp \
					http_request/request_handler.cpp \
					http_request/uri_parser.cpp \
					http_request/utility_path.cpp \
					http_request/utility_split.cpp \
					http_response/error_checking.cpp \
					http_response/response_data.cpp \
					http_response/response_generator.cpp \
					http_response/response_handler.cpp \
					http_response/response_utils.cpp \
					http_response/status_code_reason_phrase_map.cpp \
					virtual_server/search_for_file.cpp \
					virtual_server/select_location.cpp \
					virtual_server/select_virtual_server.cpp \
					virtual_server/virtual_server.cpp

SRCS		=		$(addprefix ${SRC_DIR}/, ${SRC_FILES})

OBJS		=		${SRCS:.cpp=.o}

INCLUDES	=		./includes/

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Created executable: $(NAME)"

%.o: %.cpp
	@echo "Compiling $<"
	@${CC} ${CFLAGS} -o $@ -c $<

clean:
	@echo "Removing object files"
	@rm -rf $(OBJS)

fclean: clean
	@echo "Removing executable: $(NAME)"
	@rm -rf $(NAME)

re: fclean all
