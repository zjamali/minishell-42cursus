/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 19:44:18 by mbari             #+#    #+#             */
/*   Updated: 2021/06/02 19:44:45 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

void	ft_check_digit(char	*arg)
{
	char	*error;
	int		estatus;
	int		i;

	i = 0;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			if (g_vars.history)
				g_vars.history = ft_destroy_history(g_vars.history);
			error = ft_strjoin(arg, ": numeric argument required");
			estatus = ft_put_err("exit: ", error, 255);
			free(error);
			exit(estatus);
		}
		i++;
	}
}

int	ft_check_exit(char *arg)
{
	int			i;
	char		*error;
	long long	estatus;

	i = 0;
	if (arg[i] == '-')
		i++;
	ft_check_digit(arg + i);
	estatus = ft_atoi(arg + i);
	if (estatus < 0)
	{
		if (g_vars.history)
			g_vars.history = ft_destroy_history(g_vars.history);
		error = ft_strjoin(arg, ": numeric argument required");
		estatus = ft_put_err("exit: ", error, 255);
		free(error);
		exit(estatus);
	}
	if (i == 0)
		return (estatus);
	return (estatus * -1);
}

int	ft_exit(t_args *args)
{
	int		i;

	if (args == NULL)
	{
		if (g_vars.infork == NO)
			ft_putendl_fd("exit", 1);
		exit(0);
	}
	if (g_vars.infork == NO)
		ft_putendl_fd("exit", 1);
	i = ft_check_exit(args->value);
	if (args->next != NULL)
		return (ft_put_err("exit", ": too many arguments", 1));
	if (g_vars.history)
		g_vars.history = ft_destroy_history(g_vars.history);
	exit(i);
}
