/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 18:50:32 by mbari             #+#    #+#             */
/*   Updated: 2021/03/31 19:21:44 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

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
		while ((ft_strcmp(args->value, "-n") == 0))
			args = args->next;
		ft_loop(args);
	}
	else
	{
		ft_loop(args);
		write(1, "\n", 1);
	}
	
}

void ft_pwd(t_env **head)
{
	t_env *pwd;

	pwd = ft_search_in_list(head, "PWD");
	ft_putendl_fd(pwd->value, 1);
}

void ft_env(t_env **head)
{
	t_env *temp;

	temp = *head;
	while (temp != NULL)
	{
		ft_putstr_fd(temp->name, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(temp->value, 1);
		temp = temp->next;
	}
}

void ft_export(t_env **head, t_args *args)
{
	char **var;
	t_env *sort_list;
	t_env *newnode;
	
	if (args == NULL)
	{
		sort_list = ft_sort_list(head);
		newnode = sort_list;
		while (newnode != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(newnode->name, 1);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(newnode->value, 1);
			ft_putendl_fd("\"",1);
			newnode = newnode->next;
		}
		return ;
	}
	while (args != NULL)
	{
		var = ft_split(args->value, '=');
		if ((newnode = ft_search_in_list(head, var[0])) != NULL)
		{
			ft_replaceit(head, var[0], var[1]);
			args = args->next;
		}
		else
		{
			newnode = ft_create_node(var[0], var[1]);
			ft_add_to_list(head, newnode);
			args = args->next;
		}
	}
}