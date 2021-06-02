/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:26:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/02 09:29:25 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	print_tokens(t_token *tokens_list)
{
	while (tokens_list != NULL)
	{
		write(1, PURPLE, ft_strlen(PURPLE));
		write(1, "{", 1);
		write(1, tokens_list->value, strlen(tokens_list->value));
		write(1, "}", 1);
		ft_putstr_fd("     index : ", 1);
		ft_putnbr_fd(tokens_list->index, 1);
		ft_putstr_fd("      type : ", 1);
		ft_putnbr_fd(tokens_list->type, 1);
		write(1, "\n", 1);
		tokens_list = tokens_list->next;
	}
}
