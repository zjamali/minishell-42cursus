#include <stdio.h>
#include <string.h>

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
	t_token_type type;
	char *value;
}t_token;

typedef struct s_tokens_list
{
	int index;
	t_token *token;
	t_token *next;
}t_tokens_list;
