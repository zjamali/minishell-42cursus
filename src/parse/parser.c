/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:37:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:23:22 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_redirection	*ft_create_redirection(t_token **tokens, int index)
{
	t_redirection	*redirection;

	redirection = (t_redirection *)malloc(sizeof(t_redirection));
	if (!redirection)
		return (NULL);
	redirection->index = index;
	redirection->next = NULL;
	if ((*tokens)->type == GREAT)
		redirection->type = RE_GREAT;
	else if ((*tokens)->type == DOUBLE_GREAT)
		redirection->type = RE_DOUBLE_GREAT;
	else if ((*tokens)->type == LESS)
		redirection->type = RE_LESS;
	(*tokens) = (*tokens)->next;
	redirection->file_name = ft_strdup((*tokens)->value);
	(*tokens) = (*tokens)->next;
	return (redirection);
}

t_redirection	*ft_insert_redirection(t_redirection *redirection,
				t_token **tokens, int index)
{
	t_redirection	*tmp;

	tmp = NULL;
	if (redirection == NULL)
		redirection = ft_create_redirection(tokens, index);
	else
	{
		tmp = redirection;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redirection(tokens, index);
	}
	return (redirection);
}

void	ft_insert_arg(t_args *head, t_args *current_args)
{
	t_args	*tmp;

	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_args;
}

t_args	*ft_create_arg(char *value)
{
	t_args	*arg;

	arg = (t_args *)malloc(sizeof(t_args));
	arg->value = ft_strdup(value);
	arg->next = NULL;
	arg->inside_quotes = 0;
	return (arg);
}

t_command_list	*ft_parser(t_token *tokens_list, int *status)
{
	t_command_list	*command_list;

	command_list = NULL;
	write(1, RED, ft_strlen(RED));
	if (!ft_check_syntax(tokens_list, status))
	{
		command_list = ft_create_ast(tokens_list);
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}
