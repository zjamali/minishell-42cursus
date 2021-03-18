/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 17:59:31 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/18 19:25:09 by zjamali          ###   ########.fr       */
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
		ft_putstr_fd("     index : ",1);
		ft_putnbr_fd(tokens_list->index,1);
		ft_putstr_fd("      type : ",1);
		ft_putnbr_fd(tokens_list->type,1);
		write(1,"\n",1);
		tokens_list = tokens_list->next;
	}
}

t_token	*first_token(void)
{
	t_token *new_token;
	new_token = malloc(sizeof(t_token));
	new_token->value = ft_strdup("NONE");
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

void	get_redir(t_token *tokens_list, char *line, int *j,int *index)
{
	if (line[*j] == '>') ///  get GREAT and DGREAT redirection
	{
		if (line[*j + 1] == '>')
		{
			add_token(tokens_list,DOUBLE_GREAT,ft_strdup(">>"),*index);
			*j = *j + 2;
			(*index)++;
		}
		else
		{
			add_token(tokens_list,GREAT,ft_strdup(">"),*index);
			(*j)++;
			(*index)++;
		}
	}
	else if (line[*j] == '<')// GET LESS redirction
	{
		add_token(tokens_list, LESS,ft_strdup("<"),*index);
		(*j)++;
		(*index)++;
	}
}

void	get_space_pipe_semi_redir(t_token *tokens_list, char *line, int *j,int *index)
{
	char *token;
	if (line[*j] == '|') 	/// get pipe
	{
		if (line[*j + 1] == '|')
			token = ft_strdup("||");
		else
			token = ft_strdup("|");
		add_token(tokens_list,PIPE,token,*index);
		(*index)++;
		(*j)++;
	}
	else if (line[*j] == ';') // get semi
	{
		if (line[*j + 1] == ';')
		{
			token = ft_strdup(";;");
		}
		else
			token = ft_strdup(";");
		add_token(tokens_list,SEMI,token,*index);
		(*index)++;
		(*j)++;
	}
	else
		get_redir(tokens_list,line,j,index);
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

int  check_the_beginning_of_word(int c)
{	
	if (c == 92) // begining is bach_slash "\"
			return (1);
	else if (c == 39) // begining is single_quoting ' 
			return (2);
	else if(c == 34) // // begining is double_quoting "
		return  (3);
	else // no quoting
		return (0);
}

char *get_no_quoting_word(char *line,int *i)
{
	char *word;

	int j = *i;
	while (line[j] && !ft_strrchr("\t '\"\\<>;|",line[j]))
		j++;
	word = ft_substr(line,*i,j - *i);
	*i = j;
	return word;
}

char *get_quoting_word(char *line,int *i,int quoting)
{
	int j;
	char *word;
	
	j = *i;
	word = NULL;
	if (quoting == 1) /// backslash quoting
	{
		word = ft_substr(line,*i,2);
		*i += 2;
		return word;
	}
	else if (quoting == 2)
	{
		j++; /// escape first quote
		while(line[j] && line[j] != '\'')
			j++;
		word = ft_substr(line,*i,j - *i + 1);
		*i = j + 1;
		return word;
	}
	else if (quoting == 3)
	{
		j++;
		while(line[j])
		{
			if (line[j] == '"' && line[j - 1] != '\\')
				break;
			j++;
		}
		word = ft_substr(line,*i,j - *i + 1);
		*i = j + 1;
		return word;
	}
	return word;
}

void ft_get_word(t_token *tokens_list,char *line,int *tab)
{
	int quoting;
	char *word;
	int j;
	char *tmp;
	char *tmp1;
	
	j = 0;
	quoting = -1;
	word = NULL;
	j = tab[1];
	while(line[j])
	{
		quoting = check_the_beginning_of_word(line[j]);
		if (quoting == 0) /// not quoted word 
		{
			tmp = word;
			tmp1 = get_no_quoting_word(line,&j);
			word = ft_strjoin(word,tmp1);
			free(tmp);
			free(tmp1);
			if (ft_strchr("\t ><|;",line[j]))
				break;  //  space after word 
		}
		if (quoting > 0) /// get quoted word 
		{
			tmp = word;
			tmp1 = get_quoting_word(line,&j,quoting);
			word = ft_strjoin(word,tmp1);
			free(tmp1);
			free(tmp);
			quoting = -1;
			if (line[j] == ' '|| line[j] == '\t') // get space after delimiters('" space)of word
				break;
		}
	}
	add_token(tokens_list, WORD,word,tab[3]);
	tab[1] = j;
	tab[3]++;
}

void	create_tokens_list(t_token *tokens_list, char* line)
{
	int tab[5]; // 0 = i  ; 1 = j ; k = 2; index = 3 ; quote = 4; 
	
	tab[3] = 1;
	tab[4] = 0;
	tab[0]  = 0;
	while (line[tab[0]])
	{
		tab[1] = tab[0];
		while (line[tab[1]] == ' ' || line[tab[1]] == '\t') /// escape spaces 
			tab[1]++;
		if (ft_strrchr( "|;><" , line[tab[1]]) != NULL)  //  GET space pipe semi redir 
			get_space_pipe_semi_redir(tokens_list,line,&tab[1],&tab[3]);
		if (ft_strrchr("\t <>;|", line[tab[1]]) == NULL || line[tab[1]] == 92) // GET WORD 
		{
			tab[2] = tab[1];
			ft_get_word(tokens_list,line,tab);
		}
		tab[0] = tab[1];
	}
	add_token(tokens_list,NEWLINE,ft_strdup("newline"),tab[3]);
}

t_token	*ft_lexer(char *line)
{
	t_token *tokens_list;
	tokens_list = NULL;
	tokens_list = first_token();
	create_tokens_list(tokens_list, line);
	//print_tokens(tokens_list);
	return tokens_list;
}