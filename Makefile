# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tbartocc <tbartocc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/09/02 15:18:00 by tbartocc          #+#    #+#              #
#    Updated: 2025/01/02 15:42:24 by peli             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Werror -Wextra -g3 -fsanitize=address
LDFLAGS = -lreadline -lhistory -lncurses

LIBFT = ./libft

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
		lexer/heredoc_case.c \
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

SRC = 	$(SRC_COMMON)\
		main.c \

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) --no-print-directory -C $(LIBFT)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBFT)/libft.a -o $(NAME)

%.o: %.c src.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@$(MAKE) --no-print-directory -C $(LIBFT) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) --no-print-directory -C $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re debug tester
