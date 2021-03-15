/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/15 14:52:49 by zjamali          ###   ########.fr       */
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
	ft_putstr_fd("bash: syntax error near unexpected token `",1);
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

void 	ft_check_first_token(t_token *tokens_list)
{
	t_token *first_token;

	if (tokens_list->type == NONE)
		first_token = tokens_list->next;
	else
	{
		return;
	}
	
	if (first_token->type == PIPE || first_token->type == SEMI)
	{
		ft_print_systax_error(first_token);
		ft_destoy_token_list(tokens_list);
	}
}

void ft_check_syntax(t_token *tokens_list)
{
	/// check the first token must be word or redierction
	ft_check_first_token(tokens_list);
	
}
void ft_parser(t_token *tokens_list)
{
	ft_check_syntax(tokens_list);
	
}