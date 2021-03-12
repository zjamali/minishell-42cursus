/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/12 17:54:31 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <stdio.h>
#include <string.h>

void read_command_list(char **line)
{
	get_next_line(&(*line));
}
t_token *first_token()
{
	t_token *new_token;
	new_token = malloc(sizeof(t_token));
	new_token->value = strdup("nan");
	new_token->next = NULL;
	new_token->type = NONE;
	new_token->index = 0;
	return new_token;
}

void show_prompt()
{
	write(1,"minishell$ ",strlen("minishell$ "));
}

int             ft_isdigit(int c)
{
        if (c >= '0' && c <= '9')
                return (1);
        return (0);
}

int             ft_isalpha(int c)
{
        if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
                return (1);
        return (0);
}
void add_token(t_token *token_list,t_token_type type,char *content)
{
	
	int index;
	t_token *tmp  = token_list;
	// t_token 
	/// get the last of list
			// tmp = token_list->next;
	while (tmp->next != NULL)
	{
		index = tmp->index;
		tmp = tmp->next;
	}
	//// creat PIPE token
	if (type == PIPE)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = PIPE;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	if (type == GREAT)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = PIPE;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	if (type == DOUBLE_GREAT)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = DOUBLE_GREAT;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	if (type == LESS)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = LESS;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	if (type == WORD)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = WORD;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	if (type == SEMI)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = SEMI;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}

}
char *close_token(char *line,int j,int k)
{
	return ft_substr(line,j,k - j + 1);
}
void create_tokens_list(t_token *tokens_list,char* line)
{
	//t_token *tmp;
	int i;
	int j;
	int quote;
	int k;
	char *token_value;
	//tmp = tokens_list;
	quote = 0;
	i  = 0;

	while (line[i])
	{
		j = i;
		while (line[j] == ' ') /// escape spaces 
			j++;
		i = j;
		if (line[j] == '|') 	/// get pipe
		{
			add_token(tokens_list,PIPE,"|");
			j++;
		}
		i = j;
		if (line[j] == ';') // get semi
		{
			add_token(tokens_list,SEMI,";");
			j++;
		}
		i = j;
		if (line[j] == '>') ///  get GREAT and DGREAT redirection
		{
			if (line[j + 1] == '>')
			{
				add_token(tokens_list,DOUBLE_GREAT,">>");
				j = j + 2;
			}
			else
			{
				add_token(tokens_list,GREAT,">");
				j++;
			}
		}
		i = j;
		
		if (line[j] == '<') // GET LESS redirction
		{
			add_token(tokens_list,LESS,"<");
			j++;
		}
		i = j;
		
		if(ft_strrchr("<> ;|",line[j]) == NULL) // GET WORD 
		{
			
			k = j;
			if (quote == 0 && line[j] == 34) /// double quotes in begining of token
			{
				quote = 2;
				k++;
			}
			else if (quote == 0 && line[j] == 39) /// single quotes in brgining of token
			{	
				quote = 1;
				k++;
			}
			else
			{
				quote = 3; /// no quotes in begining
			}
			
			if( quote == 1 )
			{
				while (line[k] && line[k] != 39)
					k++;
				token_value = close_token(line,j,k);
				add_token(tokens_list,WORD,token_value);
				k++;
				j = k;
				quote  = 0;
			}
			else if( quote == 2 )
			{
				while (line[k] && line[k] != 34)
					k++;
				token_value = close_token(line,j,k);
				add_token(tokens_list,WORD,token_value);
				k++;
				j = k;
				quote = 0;
			}
			else if( quote == 3 )
			{
				while (line[k] && ft_strrchr("' <>;|",line[k]) == NULL && line[k] != 34)
					k++;
				token_value= close_token(line,j,k - 1);
				add_token(tokens_list,WORD,token_value);
				j = k;
				quote = 0;
			}
		}
		i = j;
	}
}


void print_tokens(t_token *tokens_list)
{
	
	while (tokens_list != NULL)
	{
		write(1,"{",1);
		write(1,tokens_list->value,strlen(tokens_list->value));
		write(1,"}",1);
		tokens_list = tokens_list->next;
	}
}
int main()
{
	t_token *tokens_list;
	char *line;

	tokens_list = NULL;
	line = NULL;
	while (1)
	{
		show_prompt();
		read_command_list(&line);
		if ( line[1] != '\0')
			write(1, "\n",1);
		tokens_list = ft_lexer(line);
	}
}


t_token	*ft_lexer(char *line)
{
	t_token *tokens_list;	
	tokens_list = first_token();
	create_tokens_list(tokens_list,line);
	print_tokens(tokens_list);
	return tokens_list;
}