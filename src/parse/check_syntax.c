/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 18:45:13 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/02 21:46:19 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 1);
	ft_putstr_fd(token->value, 1);
	ft_putstr_fd("'\n", 1);
}

int	count_bachslashes(char *word, int *index, int back_slash)
{
	while (word[*index] == 92)
	{
		back_slash++;
		(*index)++;
	}
	return (back_slash);
}

int	ft_get_first_double_quotes(char *word, int *index, int *back_slash)
{
	int	quote;

	quote = 0;
	if (*index != 0 && word[*index - 1] != 92)
		quote += 2;
	else
	{
		if (*back_slash % 2 == 0)
			quote += 2;
	}
	(*index)++;
	*back_slash = 0;
	return (quote);
}

int	ft_get_first_single_quotes(char *word, int *index, int *back_slash)
{
	int	quote;

	quote = 0;
	if (word[*index - 1] != 92)
	{
		quote += 1;
	}
	else
	{
		if (*back_slash % 2 == 0)
		{
			quote += 1;
		}
	}
	(*index)++;
	*back_slash = 0;
	return (quote);
}

void	ft_get_close_double_quotes(char *word, int *index, int *back_slash,
																	int *quote)
{
	if (word[*index - 1] != 92)
		*quote -= 2;
	else
	{
		if (*back_slash % 2 == 0)
			*quote -= 2;
	}
	*back_slash = 0;
	(*index)++;
}

void	ft_get_close_single_quotes(char *word, int *index, int *back_slash,
																	int *quote)
{
	(*index)++;
	*quote -= 1;
	*back_slash = 0;
}

int	quote_return(int quote)
{
	if (quote != 0)
		return (1);
	else
		return (0);
}

int	ft_check_closing_quotes(char *word)
{
	int	i;
	int	quote;
	int	back_slash;

	back_slash = 0;
	i = 0;
	quote = 0;
	while (word[i])
	{
		back_slash = count_bachslashes(word, &i, back_slash);
		if (quote == 0 && word[i] == 34)
			quote = ft_get_first_double_quotes(word, &i, &back_slash);
		else if (quote == 0 && word[i] == '\'')
			quote = ft_get_first_single_quotes(word, &i, &back_slash);
		else if (quote == 2 && word[i] == 34)
			ft_get_close_double_quotes(word, &i, &back_slash, &quote);
		else if (quote == 1 && word[i] == '\'')
			ft_get_close_single_quotes(word, &i, &back_slash, &quote);
		else
		{
			i++;
			back_slash = 0;
		}
	}
	return (quote_return(quote));
}

int	ft_check_backslash(char *word)
{
	int	i;
	int	cont;

	i = 0;
	cont = 0;
	while (word[i])
		i++;
	if (word[i - 1] == '\\')
	{
		i--;
		while (word[i] == '\\')
		{
			cont++;
			i--;
		}
		if ((cont % 2) != 0)
			return (1);
		else
			return (0);
	}
	else
		return (0);
}

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

int	check_redirection(t_token *tokens_list, t_token *token, int *status)
{
	int	result;

	result = 0;
	if (token->type != WORD)
	{
		ft_print_systax_error(token);
		ft_destoy_token_list(tokens_list);
		*status = 258;
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

int	ft_check_word_semi_pipe_redir(t_token *tokens_list, t_token *token,
																	int *status)
{
	int	result;

	result = 0;
	if (token->type == NONE)
		result = check_first_token(tokens_list, token->next, status);
	else if (token->type == GREAT || token->type == DOUBLE_GREAT
		|| token->type == LESS)
		result = check_redirection(tokens_list, token->next, status);
	else if (token->type == PIPE)
		result = check_tokn_nxt_pipe(tokens_list, token->next, status);
	else if (token->type == SEMI)
		result = check_tokn_next_semi(tokens_list, token->next, status);
	else if (token->type == WORD)
		result = check_word_token(tokens_list, token, status);
	return (result);
}

int	ft_check_syntax(t_token *tokens_list, int *status)
{
	int		result;
	t_token	*tmp;

	result = 0;
	tmp = tokens_list;
	while (tmp->type != NEWLINE)
	{
		if (tmp->type == WORD && tmp->next->type == NEWLINE)
		{
			result = check_last_word_token(tokens_list, tmp, status);
			if (result)
				break ;
		}
		else
		{
			result = ft_check_word_semi_pipe_redir(tokens_list, tmp, status);
			if (result)
				break ;
		}
		tmp = tmp->next;
	}
	return (result);
}
