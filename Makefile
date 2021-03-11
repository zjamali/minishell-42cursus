NAME= minishell

SRC=  src/main.c src/get_next_line.c

INC= headers/get_next_line.h headers/minishell.h
OBJ= $(SRC:.c=.o)

CFLAGS=  -Wall -Wextra -Werror

libft:
	make -sC src/libft/


all: libft $(NAME)
	
$(NAME):
	gcc $(CFLAGS) $(SRC) 