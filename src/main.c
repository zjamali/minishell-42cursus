/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/11 19:54:26 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <stdio.h>
#include <string.h>

void read_command_list(char **line)
{
	get_next_line(&(*line));
}

void show_prompt()
{
	write(1,"minishell$",strlen("minishell$"));
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
void add_token(t_token *token_list,t_token_type type,char *content )
{
	int index;
	/// get the last of list
	while (token_list != NULL)
	{
		index = token_list->index;
		token_list = token_list->next;
	}
	
	//// creat PIPE token	
	if (type == PIPE)
	{
		token_list = malloc(sizeof(t_token));
		token_list->index = index + 1;
		token_list->type = PIPE;
		token_list->fields = NULL;
		token_list->value = content;
		token_list->next = NULL;
	}
	if (type == GREAT)
	{
		token_list = malloc(sizeof(t_token));
		token_list->index = index + 1;
		token_list->type = PIPE;
		token_list->fields = NULL;
		token_list->value = content;
		token_list->next = NULL;
	}
	if (type == DOUBLE_GREAT)
	{
		token_list = malloc(sizeof(t_token));
		token_list->index = index + 1;
		token_list->type = DOUBLE_GREAT;
		token_list->fields = NULL;
		token_list->value = content;
		token_list->next = NULL;
	}
	if (type == LESS)
	{
		token_list = malloc(sizeof(t_token));
		token_list->index = index + 1;
		token_list->type = LESS;
		token_list->fields = NULL;
		token_list->value = content;
		token_list->next = NULL;
	}
	if (type == WORD)
	{
		token_list = malloc(sizeof(t_token));
		token_list->index = index + 1;
		token_list->type = WORD;
		token_list->fields = NULL;
		token_list->value = content;
		token_list->next = NULL;
	}

}

void create_tokens_list(t_token **tokens_list,char* line)
{
	t_token *tmp;
	int i;
	int j;
	int quote;
	int k;
	char *token_value;

	tmp = *tokens_list;
	quote = 0;
	i  = 0;
	while (line[i])
	{
		
		j = i;
		/// escape spaces 
		while (line[j] == ' ')
			j++;
		i = j;
		/// get pipe
		if (line[j] == '|')
		{
			write(1,"{",1);
			write(1,"|",1);
			write(1,"}",1);
			add_token(tmp,PIPE,"|");
			j++;
		}
		i = j;
		// get semi
		if (line[j] == ';')
		{
			write(1,"{",1);
			write(1,";",1);
			write(1,"}",1);
			add_token(tmp,SEMI,";");
			j++;
		}
		i = j;
		///  get GREAT and DGREAT redirection
		if (line[j] == '>')
		{
			if (line[j + 1] == '>')
			{
				write(1,"{",1);
				write(1,">>",2);
				write(1,"}",1);
				add_token(tmp,DOUBLE_GREAT,">>");
				j = j + 2;
			}
			else
			{
				write(1,"{",1);
				write(1,">",1);
				write(1,"}",1);
				add_token(tmp,GREAT,">");
				j++;
			}
		}
		i = j;
		// GET LESS redirction
		if (line[j] == '<')
		{
			write(1,"{",1);
			write(1,"<",1);
			write(1,"}",1);
			add_token(tmp,LESS,"<");
			j++;
		}
		i = j;
		// GET WORD 
		if(ft_isdigit(line[j]) ||ft_isalpha(line[j]) || line[j] == 34 /* " */ 
			|| line[j] == 39 /* ' */ || line[j] == 47 /* \ */)
		{
			k = j;
			while (line[k] && line[k] != ' ')
			{
				if (line[k] == 39)
				{
					quote = 1;
					k++;
					while (line[k] && line[k] != 34)
						k++; //// increase intill find single quotes 
					token_value = ft_substr(line,j,k - j + 1); // get form j to k
					write(1,"{",1);
					write(1,token_value,ft_strlen(token_value));
					write(1,"}",1);
					//add_token(tmp,WORD,token_value);
					k++;
					j = k;
				}
				else if (line[k] == 34)
				{
					quote = 2;
					k++;
					while (line[k] && line[k] != 34)
						k++; //// increase intill find double quotes 
					token_value = ft_substr(line,j,k - j + 1); // get form j to k
					write(1,"zahya",ft_strlen("zahya"));	
					write(1,"{",1);
					write(1,token_value,ft_strlen(token_value));
					write(1,"}\n",2);
					k++; // get the character after "
					//add_token(tmp,WORD,token_value);
					j = k;
				}
			}
		}
		i = j;		
	}
	
}

void begin_token_list(t_token **tokens_list)
{
	*tokens_list = malloc(sizeof(t_token));
	(*tokens_list)->value = strdup("nan");
	(*tokens_list)->next = NULL;
	(*tokens_list)->type = NONE;
	(*tokens_list)->index = 0;
}
void print_tokens(t_token *tokens_list)
{
	while (tokens_list != NULL)
	{
		write(1,"{",1);
		write(1,tokens_list->value,strlen(tokens_list->value));
		printf("--> %d",tokens_list->type);
		write(1,"}",1);
		tokens_list = tokens_list->next;
	}
}
int main()
{
	t_token *tokens_list;
	begin_token_list(&tokens_list);
	char *line;
	line = NULL;
	while (1)
	{
		show_prompt();
		read_command_list(&line);
		write(1,line,strlen(line));
		if ( line[1] != '\0')
			write(1, "\n",1);
		write(1,"---line---\n",strlen("---line---\n"));
		create_tokens_list(&tokens_list,line);
		//print_tokens(tokens_list);
	}
}
