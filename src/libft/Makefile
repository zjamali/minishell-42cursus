# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zjamali <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/10/21 18:57:04 by zjamali           #+#    #+#              #
#    Updated: 2019/11/13 17:59:26 by zjamali          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME= libft.a

SRC=	ft_calloc.c	ft_isascii.c	ft_memccpy.c	ft_memcpy.c 	ft_strchr.c \
		ft_strlcpy.c	ft_strnstr.c	ft_toupper.c	ft_atoi.c		ft_isalnum.c \
		ft_isdigit.c	ft_memchr.c		ft_memmove.c	ft_strdup.c \
		ft_strlen.c		ft_strrchr.c	ft_bzero.c		ft_isalpha.c	ft_isprint.c \
		ft_memcmp.c		ft_memset.c		ft_strlcat.c	ft_strncmp.c	ft_tolower.c \
		ft_substr.c		ft_strjoin.c	ft_strtrim.c	ft_split.c		ft_itoa.c \
		ft_strmapi.c	ft_putchar_fd.c	ft_putstr_fd.c	ft_putendl_fd.c	ft_putnbr_fd.c \

SRCB= ft_lstnew_bonus.c     ft_lstadd_front_bonus.c   ft_lstsize_bonus.c    ft_lstlast_bonus.c \
	  ft_lstadd_back_bonus.c	ft_lstdelone_bonus.c	ft_lstmap_bonus.c	ft_lstiter_bonus.c	ft_lstclear_bonus.c

LIB= libft.h
OBJ= $(SRC:.c=.o)
OBJB= $(SRCB:.c=.o)
CFLAGS=  -Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(OBJ) $(LIB)
	@gcc -c $(SRC)
	@ar rc $(NAME) $(OBJ) 
	@ranlib $(NAME)

clean:
	@rm -f $(OBJ) $(OBJB)

fclean: clean 
	@rm -f $(NAME)

re: fclean all

bonus: $(OBJB) $(OBJ) $(LIB)
	@gcc  -c $(SRC) $(SRCB)
	@ar rc $(NAME) $(OBJB) $(OBJ)
	@ranlib $(NAME)
