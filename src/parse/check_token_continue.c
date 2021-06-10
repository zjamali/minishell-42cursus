/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token_continue.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 18:47:36 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:48:28 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_print_systax_error(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 1);
	ft_putstr_fd(token->value, 1);
	ft_putstr_fd("'\n", 1);
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
