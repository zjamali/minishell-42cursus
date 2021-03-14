/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 17:59:31 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/14 18:00:00 by zjamali          ###   ########.fr       */
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

void	add_token(t_token *token_list,t_token_type type,char *content,int index)
{
	t_token *tmp  = token_list;

	while (tmp->next != NULL) /// get last node of list_tokens
		tmp = tmp->next;
	
	tmp->next = malloc(sizeof(t_token));
	tmp->next->index = index;
	tmp->next->type = type;
	tmp->next->fields = NULL;
	tmp->next->value = content;
	tmp->next->next = NULL;
}

char	*ft_close_token(char *line,int j,int k)
{
	return ft_substr(line,j,k - j + 1);
}

void	get_space_pipe_semi_redir(t_token *tokens_list, char *line, int *j,int *index)
{
		if (line[*j] == '|') 	/// get pipe
		{
			add_token(tokens_list,PIPE,"|",*index);
			(*index)++;
			(*j)++;
		}
		else if (line[*j] == ';') // get semi
		{
			add_token(tokens_list,SEMI,";",*index);
			(*index)++;
			(*j)++;
		}
		else if (line[*j] == '>') ///  get GREAT and DGREAT redirection
		{
			if (line[*j + 1] == '>')
			{
				add_token(tokens_list,DOUBLE_GREAT,">>",*index);
				*j = *j + 2;
				(*index)++;
			}
			else
			{
				add_token(tokens_list,GREAT,">",*index);
				(*j)++;
				(*index)++;
			}
		}
		else if (line[*j] == '<') // GET LESS redirction
		{
			add_token(tokens_list, LESS, "<",*index);
			(*j)++;
			(*index)++;
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

void ft_get_word(t_token *tokens_list,char *line,int *tab)
{
	if( tab[4] == 1 )
	{
		while (line[tab[2]] && line[tab[2]] != 39)
			(tab[2])++;
		add_token(tokens_list,WORD,ft_close_token(line,tab[1],tab[2]),tab[3]);
		(tab[2])++;
		tab[1] = tab[2];
		tab[4]  = 0;
		(tab[3])++;
	}
	else if(tab[4] == 2 )
	{
		while (line[tab[2]] && line[tab[2]] != 34)
			(tab[2])++;
		add_token(tokens_list,WORD,ft_close_token(line,tab[1],tab[2]),tab[3]);
		(tab[2])++;
		tab[1] = tab[2];
		tab[4] = 0;
		(tab[3])++;
	}
	else if(tab[4] == 3 )
	{
		while (line[tab[2]] && ft_strrchr("' <>;|",line[tab[2]]) == NULL && line[tab[2]] != 34)
			(tab[2])++;
		add_token(tokens_list, WORD,ft_close_token(line, tab[1], tab[2] - 1),tab[3]);
		tab[1] = tab[2];
		tab[4] = 0;
		(tab[3])++;
	}	
}
void get_backslash(t_token *tokens_list,char *line,int *j,int *index)
{
	char *word;
	int backslach_token;
	int k ;

	k = *j;
	word = NULL;
	backslach_token = 0;
	while(line[k] && line[k] == 92)
	{
		if (line[k + 1])
		{
			word = ft_strjoin(word,ft_substr(line,k + 1,1));
			k = k + 2;
		}
		else
		{
			backslach_token = 1;
			k++;
		}	
	}
	add_token(tokens_list,WORD,word,*index);
	(*index)++;
	if (backslach_token == 1)
		add_token(tokens_list,WORD,"\\",*index);
	(*index)++;
	*j = k;
}
void	create_tokens_list(t_token *tokens_list, char* line)
{
	int tab[5]; // 0 = i  ; 1 = j ; k = 2; index = 3 ; quote = 4; 
	
	//tab = malloc(sizeof(int)*5);
	
	tab[3] = 1;
	tab[4] = 0;
	tab[0]  = 0;
	while (line[tab[0]])
	{
		tab[1] = tab[0];
		while (line[tab[1]] == ' ') /// escape spaces 
			tab[1]++;
		if (ft_strrchr( "|;><" , line[tab[1]]) != NULL)  //  GET space pipe semi redir 
			get_space_pipe_semi_redir(tokens_list,line,&tab[1],&tab[3]);
		if (line[tab[1]] == 92)
			get_backslash(tokens_list,line,&tab[1],&tab[3]);
		if (ft_strrchr(" <>;|", line[tab[1]]) == NULL && line[tab[1]] != 92) // GET WORD 
		{
			tab[2] = tab[1];
			ft_check_the_begining_of_word(line, &tab[2], &tab[1], &tab[4]);
			//ft_get_word(tokens_list, line, &tab[4], &tab[2], &tab[1],&tab[3]);
			ft_get_word(tokens_list, line, tab);
		}
		tab[0] = tab[1];
	}
	add_token(tokens_list,NEWLINE,"\\n",tab[3]);
}

t_token	*ft_lexer(char *line)
{
	t_token *tokens_list;	
	tokens_list = first_token();
	create_tokens_list(tokens_list, line);
	print_tokens(tokens_list);
	return tokens_list;
}