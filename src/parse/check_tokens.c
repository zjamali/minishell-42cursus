/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 18:30:14 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:48:24 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	check_first_token(t_token *tokens_list, t_token *first_token, int *status)
{
	int	result;

	result = 0;
	if (first_token->type == PIPE || first_token->type == SEMI)
	{
		ft_print_systax_error(first_token);
		ft_destoy_token_list(tokens_list);
		result = 1;
		*status = 258;
	}
	else if (first_token->type == NEWLINE)
	{
		ft_destoy_token_list(tokens_list);
		*status = 0;
		result = 1;
	}
	return (result);
}

int	check_tokn_nxt_pipe(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type == PIPE || token->type == SEMI)
	{
		ft_print_systax_error(token);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	else if (token->type == NEWLINE)
	{
		ft_putstr_fd("minishell: syntax error multiple line not allowed\n", 1);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}

int	check_last_word_token(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (ft_check_backslash(token->value))
	{
		ft_putstr_fd("minishell: syntax error multiple line not allowed\n", 1);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	if (!result && ft_check_closing_quotes(token->value))
	{
		ft_putstr_fd("minishell: syntax error multiple line not allowed\n", 1);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}

int	check_word_token(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (ft_check_closing_quotes(token->value))
	{
		ft_putstr_fd("minishell: syntax error multiple line not allowed\n", 1);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}

int	check_tokn_next_semi(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type == PIPE || token->type == SEMI)
	{
		ft_print_systax_error(token);
		ft_destoy_token_list(tokens_list);
		*status = 258;
		result = 1;
	}
	return (result);
}
