/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_debug.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/01 17:37:36 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/03 19:54:13 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_print_lines_list(t_lines_list *lines_list)
{
	while (lines_list)
	{
		ft_putstr_fd("\nline-> ", 1);
		if (lines_list->char_list)
			ft_print_char_list(lines_list->char_list);
		else
			ft_putstr_fd("ana khawya",1);
		ft_putstr_fd("\thistory: ",1);
		ft_putnbr_fd(lines_list->history,1);
		ft_putstr_fd("\n", 1);
		lines_list = lines_list->next;
	}
}

void ft_print_lines_list_all(t_lines_list *lines_list)
{
	t_lines_list *tmp ;

	tmp = lines_list;
	
	if (tmp->prev != NULL)
	{
		while (tmp->prev != NULL)
			tmp = tmp->prev;
	}
	if (tmp)
		ft_print_lines_list(tmp);
}