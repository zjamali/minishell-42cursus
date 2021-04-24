/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 18:50:32 by mbari             #+#    #+#             */
/*   Updated: 2021/04/24 16:33:35 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

void	 ft_loop(t_args *args)
{
	while (args != NULL)
	{
		if (args->value != NULL)
			ft_putstr_fd(args->value, 1);
		else
			ft_putchar_fd(' ', 1);
		if (args->next != NULL)
			ft_putchar_fd(' ', 1);
		args = args->next;
	}
}

int ft_isitn(char *arg)
{
	int i;
	
	i = 0;
	while (arg[i] == 'n' && arg[i])
		i++;
	if (arg[i] != '\0')
		return (0);
	return (1);
}

int	 ft_echo(t_args *args)
{
	if (args == NULL || args->value ==  NULL)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	if (args->value[0] == '-')
	{
		while (ft_isitn(args->value + 1))
			args = args->next;
		ft_loop(args);
		// while ((ft_strcmp(args->value, "-n") == 0))
		// 	args = args->next;
		// ft_loop(args);
	}
	else
	{
		ft_loop(args);
		write(1, "\n", 1);
	}
	return (0);
}

int	 ft_pwd(t_env **head)
{
	t_env *pwd;

	pwd = ft_search_in_list(head, "PWD");
	ft_putendl_fd(pwd->value, 1);
	return (0);
}

int	 ft_env(t_env **head)
{
	t_env *temp;
 /// -nnnn
	temp = *head;
	while (temp != NULL)
	{
		ft_putstr_fd(temp->name, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(temp->value, 1);
		temp = temp->next;
	}
	return (0);
}

int	 ft_export(t_env **head, t_args *args)
{
	char **split;
	t_env *sort_list;
	t_env *newnode;
	int join;
	
	join = NO;
	if (args == NULL)
	{
		sort_list = ft_sort_list(head);
		newnode = sort_list;
		while (newnode != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_fd(newnode->name, 1);
			if (newnode->value != NULL)
			{
				ft_putstr_fd("=\"", 1);
				ft_putstr_fd(newnode->value, 1);
				ft_putchar_fd('"',1);
			}
			ft_putchar_fd('\n', 1);
			newnode = newnode->next;
		}
		return (0);
	}
	while (args != NULL)
	{
		split = my_split(args->value);
		if (split[0][ft_strlen(split[0]) - 1] == '+')
		{
			split[0][ft_strlen(split[0]) - 1] = '\0';
			join = YES;
		}
		if ((newnode = ft_search_in_list(head, split[0])) != NULL)
		{
			if (join == YES)
				split[1] = ft_strjoin(newnode->value, split[1]);
			ft_replaceit(head, split[0] , split[1]);
			args = args->next;
		}
		else
		{
			newnode = ft_create_node(split[0], split[1]);
			ft_add_to_list(head, newnode);
			args = args->next;
		}
	}
	return (0);
}

int ft_unset(t_args *args, t_env **head)
{
	int ret;
	
	ret = 0;
	while (args != NULL)
	{
		if (args->value == NULL)
			ret = ft_put_err("`'", ": not a valid identifier", 1);
		else
			ft_delete_from_list(head, args->value);
		args = args->next;
	}
	return (ret);
}

int ft_cd(t_args *args, t_env **head)
{
	char *buff;
	char *dir;
	t_env *temp;
	
	buff = NULL;
	if (args == NULL)
	{
		temp = ft_search_in_list(head, "HOME");
		dir = temp->value;
	}
	else if (!(ft_strcmp(args->value, "-")))
	{
		temp = ft_search_in_list(head, "OLDPWD");
		dir = temp->value;
	}
	else
		dir = args->value;
	if (chdir(dir) == -1)
		return (ft_put_err("cd", ": No such file or directory", 1));
	temp = ft_search_in_list(head, "PWD");
	ft_replaceit(head, "OLDPWD", temp->value);
	ft_replaceit(head, "PWD", getcwd(buff, 100));
	return (0);
}

int	 ft_exit(t_args *args)
{
	if (args == NULL)
		exit(0);
	if (args->next != NULL)
		return (ft_put_err("exit", ": too many arguments", 1));
	exit(ft_atoi(args->value));
}