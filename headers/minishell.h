#include <stdio.h>
#include <string.h>
#include "../src/libft/libft.h"
#include "get_next_line.h"

typedef enum e_token_type{
	NONE,
	WORD,
	SEMI,
	GREAT,
	LESS,
	DOUBLE_GREAT,
	PIPE,
	NEWLINE,
} t_token_type;

typedef struct s_token
{
	int index;
	t_token_type type;
	char *value;
	char *fields;
	struct s_token *next;
}t_token;


t_token	*ft_lexer(char *line);