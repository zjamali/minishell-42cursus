/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 17:59:31 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/12 19:54:37 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/lexer.h"


void	print_tokens(t_token *tokens_list)
{
	while (tokens_list != NULL)
	{
		write(1,"{",1);
		write(1,tokens_list->value,strlen(tokens_list->value));
		write(1,"}",1);
		ft_putnbr_fd(tokens_list->index,1);
		write(1,"\n",1);
		tokens_list = tokens_list->next;
	}
}

t_token	*first_token(void)
{
	t_token *new_token;
	new_token = malloc(sizeof(t_token));
	new_token->value = strdup("nan");
	new_token->next = NULL;
	new_token->type = NONE;
	new_token->index = 0;
	return (new_token);
}

void	add_token(t_token *token_list,t_token_type type,char *content)
{

	int index;
	t_token *tmp  = token_list;

	index = tmp->index;
	while (tmp->next != NULL) /// get last node of list_tokens
	{
		index = tmp->index;
		tmp = tmp->next;
	}
	
	if (type == PIPE) //// creat PIPE token
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = PIPE;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	else if (type == GREAT)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = PIPE;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	else if (type == DOUBLE_GREAT)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = DOUBLE_GREAT;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	else if (type == LESS)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = LESS;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	else if (type == WORD)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = WORD;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
	else if (type == SEMI)
	{
		tmp->next = malloc(sizeof(t_token));
		tmp->next->index = index + 1;
		tmp->next->type = SEMI;
		tmp->next->fields = NULL;
		tmp->next->value = content;
		tmp->next->next = NULL;
	}
}

char	*ft_close_token(char *line,int j,int k)
{
	return ft_substr(line,j,k - j + 1);
}

void	get_space_pipe_semi_redir(t_token *tokens_list, char *line, int *j)
{
		if (line[*j] == '|') 	/// get pipe
		{
			add_token(tokens_list,PIPE,"|");
			(*j)++;
		}
		else if (line[*j] == ';') // get semi
		{
			add_token(tokens_list,SEMI,";");
			(*j)++;
		}
		else if (line[*j] == '>') ///  get GREAT and DGREAT redirection
		{
			if (line[*j + 1] == '>')
			{
				add_token(tokens_list,DOUBLE_GREAT,">>");
				*j = *j + 2;
			}
			else
			{
				add_token(tokens_list,GREAT,">");
				(*j)++;
			}
		}
		else if (line[*j] == '<') // GET LESS redirction
		{
			add_token(tokens_list, LESS, "<");
			(*j)++;
		}	
}

void	ft_check_the_begining_of_word(char *line, int *k, int *j, int *quote)
{
	if (*quote == 0 && line[*j] == 34) /// double quotes in begining of token
	{
		*quote = 2;
		(*k)++;
	}
	else if (*quote == 0 && line[*j] == 39) /// single quotes in brgining of token
	{	
		*quote = 1;
		(*k)++;
	}
	else
		*quote = 3; /// no quotes in begining	
}

void	ft_get_word(t_token *tokens_list,char *line,int *quote,int *k,int *j)
{
	if( *quote == 1 )
	{
		while (line[*k] && line[*k] != 39)
			(*k)++;
		add_token(tokens_list,WORD,ft_close_token(line,*j,*k));
		(*k)++;
		*j = *k;
		*quote  = 0;
	}
	else if( *quote == 2 )
	{
		while (line[*k] && line[*k] != 34)
			(*k)++;
		add_token(tokens_list,WORD,ft_close_token(line,*j,*k));
		(*k)++;
		*j = *k;
		*quote = 0;
	}
	else if( *quote == 3 )
	{
		while (line[*k] && ft_strrchr("' <>;|",line[*k]) == NULL && line[*k] != 34)
			(*k)++;
		add_token(tokens_list, WORD,ft_close_token(line, *j, *k - 1));
		*j = *k;
		*quote = 0;
	}
}
void	create_tokens_list(t_token *tokens_list, char* line)
{
	int i;
	int j;
	int quote;
	int k;
	
	quote = 0;
	i  = 0;
	while (line[i])
	{
		j = i;
		while (line[j] == ' ') /// escape spaces 
			j++;
		if (ft_strrchr( "|;><" , line[j]) != NULL)  //  GET space pipe semi redir 
			get_space_pipe_semi_redir(tokens_list,line,&j);
		if (ft_strrchr(" <>;|", line[j]) == NULL) // GET WORD 
		{
			k = j;
			write(1,"hna",3);
			ft_check_the_begining_of_word(line, &k, &j, &quote);
			ft_get_word(tokens_list, line, &quote, &k, &j);
		}
		i = j;
	}
}

t_token	*ft_lexer(char *line)
{
	t_token *tokens_list;	
	tokens_list = first_token();
	create_tokens_list(tokens_list, line);
	print_tokens(tokens_list);
	return tokens_list;
}