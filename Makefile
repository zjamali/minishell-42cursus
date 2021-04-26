NAME= minishell

SRC_EXECUTE= src/execution/builtins.c  src/execution/execution.c src/execution/list.c src/execution/redirection.c

SRC_PARSE=   src/parse/get_next_line.c src/parse/lexer.c src/parse/parser.c src/parse/expansion.c src/readline/readline.c

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
	@echo "██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     "
	@echo "██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗"
	@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝"
	@echo "Compilation of minishell:  \033[1;32mOK\033[m"
	@$(COM) $(CFLAGE) $(SRC_MAIN) $(SRC_PARSE) $(SRC_EXECUTE) $(LIBFT) -g -lncurses 