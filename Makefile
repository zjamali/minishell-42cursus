# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/02 16:01:57 by mbari             #+#    #+#              #
#    Updated: 2021/06/10 20:45:36 by zjamali          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= minishell

SRC_EXECUTE= src/execution/*

SRC_PARSE=    src/lexer/lexer.c src/lexer/lexer_debug.c src/lexer/lexer_get_tokens.c src/lexer/lexer_get_tokens_op.c src/lexer/lexer_get_tokens_word.c\
				 src/parse/parser.c src/parse/parser_debug.c src/parse/check_syntax.c\
				 src/expansion/expansion.c \
				 src/readline/readline.c \
				 src/last_arg.c \
				  src/minishell.c\
				  src/parse/destoy_nodes.c\
				  src/parse/create_ast.c\
				  src/parse/create_nodes.c\
				  src/parse/check_tokens.c\
				  src/parse/check_word.c \
				  src/parse/check_token_continue.c \
				  src/expansion/utils.c\
				  src/expansion/expand_command.c\
				  src/expansion/expand_non_qoutes_word.c\
				  src/expansion/expand_dble_qoutes_word.c\
				  src/expansion/expand_special_params.c\
				  src/expansion/expand_special_params_in_quotes.c\
				  src/expansion/expand_args.c\
				  src/expansion/expand_word.c\
				  src/expansion/expand_redirction.c\
				  src/expansion/utils_continue.c\
				  src/readline/chars_list_rest.c\
				  src/readline/chars_list.c\
				  src/readline/get_input.c  \
				  src/readline/history.c\
				  src/readline/line_nodes.c\
				  src/readline/terminal_config.c\
				 #src/readline/get_next_line.c

SRC_MAIN= src/main.c 

LIBFT= src/libft/libft.a

# *****     flags     ***** #

COM= gcc
CFLAGE= -Wall -Wextra -Werror

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