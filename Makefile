NAME= minishell

SRC=  src/main.c src/get_next_line.c src/lexer.c src/parser.c 

INC= headers/get_next_line.h headers/minishell.h
OBJ= $(SRC:.c=.o)

CFLAGS=  -Wall -Wextra -Werror

libft:
	make -sC src/libft/


all: libft $(NAME)

$(NAME):
	gcc $(CFLAGS) $(SRC) src/libft/libft.a