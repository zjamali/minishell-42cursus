/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/15 19:23:10 by zjamali          ###   ########.fr       */
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
	write(1,"minishell$ ",strlen("minishell$ "));
}
void ft_parser(t_token *tokens_list);
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
		ft_parser(tokens_list);
	}
}
void ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",1);
	ft_putstr_fd(token->value,1);
	ft_putstr_fd("'\n",1);
}

void ft_destoy_token_list(t_token *tokens_list)
{
	t_token *tmp;

	tmp = NULL;
	while (tokens_list->type != NEWLINE)
	{
		tmp = tokens_list;
		tokens_list = tokens_list->next;
		free(tmp);
		tmp = NULL;
	}
	
	if (tokens_list->type == NEWLINE)
	{
		//free(tokens_list->value);
		free(tokens_list);
		tokens_list = NULL;
		ft_putstr_fd("token_list freed\n",1);
	}
}

int ft_check_closing_quotes(char *word)
{
	int i ;
	int quote;
	i = 0;
	quote = 0;
	while (word[i])
	{
		if (quote == 0 && word[i] == 34 && word[i - 1] != 92)
		{
			quote += 2;
			i++;
		}
		else if (quote == 0 && word[i] == '\''  && word[i - 1] != 92)
		{
			quote = 1;
			i++;
		}
		else if (quote == 2 && word[i] == 34  && word[i - 1] != 92)
		{
			quote -= 2;
			i++;
		}
		else if (quote == 1 && word[i] == '\''  && word[i - 1] != 92)
		{
			i++;
			quote -= 1;
		}
		else
			i++;
	}
	if (quote != 0)
		return 1;
	else
		return 0;
}
int ft_check_backslash(char *word)
{
	int i;
	int cont;
	
	i = 0;
	cont = 0;
	while (word[i])
		i++;
	if (word[i - 1] == '\\')
	{
		i--;
		while(word[i] == '\\')
		{
			cont++;
			i--;
		}
		if ((cont % 2) != 0)
			return 1;
		else
		{
			return 0;
		}
	}
	else
		return (0);
}
void ft_check_syntax(t_token *tokens_list)
{
	
	t_token *tmp;
	
	tmp = tokens_list;
	while(tmp->type != NEWLINE)
	{
		/*** check the first token must be word or redierction ***/
		if (tmp->type == NONE)
		{
			if (tmp->next->type == PIPE || tmp->next->type == SEMI)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				break; //// for dont get segfault  in tmp = tmp->next;
			}
			else if (tmp->next->type == NEWLINE)
			{
				ft_destoy_token_list(tokens_list);
				break; //// for dont get segfault  in tmp = tmp->next;
			}
		}
		/*** if token is redirection ***/
		else if (tmp->type == GREAT || tmp->type == DOUBLE_GREAT || tmp->type == LESS)
		{
			if (tmp->next->type != WORD)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				break; /// for egfault
			}
		}
		else if (tmp->type == PIPE)
		{
			if (tmp->next->type == PIPE || tmp->next->type == SEMI)
			{
				ft_print_systax_error(tmp->next);
				ft_destoy_token_list(tokens_list);
				break; /// for egfault
			}
			else if (tmp->next->type == NEWLINE)
			{
				ft_print_systax_error(tmp);
				ft_destoy_token_list(tokens_list);
				break; // for segfqult
			}	
		}
		else if (tmp->type == WORD && tmp->next->type == NEWLINE)
		{
			if (ft_check_closing_quotes(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				break;
			}

			if (ft_check_backslash(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				break;
			}
		}
		else if (tmp->type == WORD) ///// check multiline
		{
			if (ft_check_closing_quotes(tmp->value))
			{
				ft_putstr_fd("minishell: syntax error multiple line not allowed\n",1);
				ft_destoy_token_list(tokens_list);
				break;
			}
		}
		tmp = tmp->next;
	}
}
void ft_parser(t_token *tokens_list)
{
	ft_check_syntax(tokens_list);
}