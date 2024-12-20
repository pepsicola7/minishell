# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/02 15:18:00 by tbartocc          #+#    #+#              #
#    Updated: 2024/12/18 17:49:41 by tbartocc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address
LDFLAGS = -lreadline -lhistory -lncurses

PRINTF = ./Printf/
LIBFT = ./Printf/Libft/

SRC_COMMON =	builtins/cd.c \
				builtins/echo.c \
				builtins/env.c \
				builtins/exit.c \
				builtins/export.c \
				builtins/pwd.c \
				builtins/unset.c \
				excutor/excutor.c \
				excutor/redirection.c \
				excutor/exc_utile.c\
				lexer/env_expansion.c \
				lexer/lexer.c \
				lexer/quote_handlers.c \
				lexer/redirection_handlers.c \
				lexer/signal_handlers.c \
				lexer/token.c \
				parser/handle_cases.c \
				parser/parser.c \
				add_env.c \
				builtin.c \
				free.c \
				get_env.c \
				get_next_line.c \
				get_next_line_utils.c \

SRC = 	$(SRC_COMMON)\
		main.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o: %.c src.h
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	@$(MAKE) --no-print-directory -C $(PRINTF)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(PRINTF)libftprintf.a -o $(NAME)

clean:
	@rm -f $(OBJ)
	@$(MAKE) --no-print-directory -C $(PRINTF) clean
	@$(MAKE) --no-print-directory -C $(LIBFT) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(PRINTF) fclean
	@$(MAKE) --no-print-directory -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re debug tester