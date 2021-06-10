/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_pwd_env.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 18:50:32 by mbari             #+#    #+#             */
/*   Updated: 2021/06/05 20:32:25 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

void	ft_loop(t_args *args)
{
	while (args != NULL)
	{
		if (args->value != NULL)
			ft_putstr_fd(args->value, 1);
		if (args->next != NULL)
			ft_putchar_fd(' ', 1);
		args = args->next;
	}
}

int	ft_isitn(char *arg)
{
	int		i;

	i = 0;
	while (arg[i] == 'n' && arg[i])
		i++;
	if (arg[i] != '\0')
		return (0);
	return (1);
}

int	ft_echo(t_args *args)
{
	int	new_line;

	new_line = YES;
	if (args == NULL)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	while (args && args->value != NULL && args->value[0] == '-'
		&& args->value[1] == 'n')
	{
		new_line = NO;
		if (args && ft_isitn(args->value + 1))
			args = args->next;
		else
			break ;
	}
	ft_loop(args);
	if (new_line == YES)
		write(1, "\n", 1);
	return (0);
}

int	ft_pwd(t_env **head)
{
	char	*buff;
	t_env	*temp;
	char	*pwd;

	buff = NULL;
	pwd = getcwd(buff, 100);
	if (pwd)
		ft_putendl_fd(pwd, 1);
	else
	{
		temp = ft_search_in_list(head, "PWD");
		if (temp)
			ft_putendl_fd(temp->value, 1);
	}
	free(pwd);
	return (0);
}

int	ft_env(t_env **head)
{
	t_env	*temp;

	temp = *head;
	while (temp != NULL)
	{
		if (temp->value != NULL)
		{
			ft_putstr_fd(temp->name, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(temp->value, 1);
		}
		temp = temp->next;
	}
	return (0);
}
