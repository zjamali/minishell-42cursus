/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 18:50:32 by mbari             #+#    #+#             */
/*   Updated: 2021/03/26 19:57:00 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/execution.h"

void ft_loop(t_args *args)
{
	while (args != NULL)
	{
		ft_putstr_fd(args->value, 1);
		if (args->next != NULL)
			ft_putchar_fd(' ', 1);
		args = args->next;
	}
}

void ft_echo(t_args *args)
{
	if (!(ft_strcmp(args->value, "-n")))
	{
		if (!(ft_strcmp(args->value, "-n")))
			args = args->next;
		ft_loop(args->next);
	}
	else
	{
		ft_loop(args);
		write(1, "\n", 1);
	}
	
}