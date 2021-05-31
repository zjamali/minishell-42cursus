# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/02 16:01:57 by mbari             #+#    #+#              #
#    Updated: 2021/05/31 18:46:51 by zjamali          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell

SRC_EXECUTE= src/execution/cd_exit.c src/execution/echo_pwd_env.c src/execution/export_unset.c src/execution/execution.c src/execution/list.c src/execution/redirection.c

SRC_PARSE=    src/lexer/lexer.c src/lexer/lexer_debug.c src/lexer/lexer_get_tokens.c src/lexer/lexer_get_tokens_op.c src/lexer/lexer_get_tokens_word.c\
				 src/parse/parser.c src/parse/parser_debug.c src/parse/check_syntax.c\
				 src/expansion/expansion.c \
				 src/readline/readline.c \
				 src/readline/get_next_line.c

SRC_MAIN= src/main.c 

LIBFT= src/libft/libft.a

# *****     flags     ***** #

COM= gcc
CFLAGE= #-Wall -Wextra -Werror

all: libft $(NAME)

libft:
	@make -sC src/libft/

$(NAME):
	@echo "         Made by : \033[1;91mzjamali\033[m and \033[1;91mmbari\033[m"
	@echo "███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     "
	@echo "████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     "
	@echo "██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     "
	@echo "██║╚██╔╝██║██║██║╚█f█╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "Compilation of minishell:  \033[1;32mOK\033[m"
	@$(COM) -g $(CFLAGE) $(SRC_MAIN) $(SRC_PARSE) $(SRC_EXECUTE) $(LIBFT) -g -lncurses