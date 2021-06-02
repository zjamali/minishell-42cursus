/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:32:24 by mbari             #+#    #+#             */
/*   Updated: 2021/05/31 11:56:17 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

int	ft_replace_pwd(t_env **head, t_env *temp, char *dir)
{
	char	*new_pwd;
	char	*buff;

	buff = NULL;
	new_pwd = getcwd(buff, 100);
	if (new_pwd)
		ft_replaceit(head, "PWD", new_pwd);
	else
		ft_replaceit(head, "PWD", ft_strjoin(temp->value,
				ft_strjoin("/", dir)));
	free(new_pwd);
	return (0);
}

int	ft_change_dir(t_args *args, t_env **head, char *dir)
{
	char	*buff;
	char	*error;
	char	*join;
	t_env	*temp;

	buff = NULL;
	temp = ft_search_in_list(head, "PWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("PWD", ""));
	temp = ft_search_in_list(head, "OLDPWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("OLDPWD", ""));
	if (chdir(dir) == -1)
	{
		join = ft_strjoin(": ", strerror(errno));
		error = ft_strjoin(dir, join);
		free(join);
		ft_put_err("cd: ", error, 1);
		free(error);
		return (1);
	}
	buff = getcwd(buff, 100);
	if (!buff)
	{
		error = ft_strjoin("cd: ", "error \
					retrieving current directory: getcwd: \
					cannot access parent directories: ");
		ft_put_err(error, strerror(errno), 1);
		free(error);
	}
	free(buff);
	temp = ft_search_in_list(head, "PWD");
	ft_replaceit(head, "OLDPWD", temp->value);
	return (ft_replace_pwd(head, temp, dir));
}

int	ft_cd(t_args *args, t_env **head)
{
	char	*dir;
	t_env	*temp;

	if (args == NULL)
	{
		temp = ft_search_in_list(head, "HOME");
		if (temp == NULL)
			return (ft_put_err("cd:", " HOME not set", 1));
		dir = temp->value;
	}
	else if (args->next != NULL)
		return (ft_put_err("cd:", " too many arguments", 1));
	else if (!(ft_strcmp(args->value, "-")))
	{
		temp = ft_search_in_list(head, "OLDPWD");
		if (temp == NULL)
			return (ft_put_err("cd:", " OLDPWD not set", 1));
		ft_putendl_fd(temp->value, 1);
		dir = temp->value;
	}
	else
		dir = args->value;
	return (ft_change_dir(args, head, dir));
}

int	ft_check_exit(char *arg)
{
	int			i;
	char		*error;
	long long	estatus;

	i = 0;
	if (arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			if (g_vars.history)
				g_vars.history = ft_destroy_history(g_vars.history);
			error = ft_strjoin(arg, ": numeric argument required");
			estatus = ft_put_err("exit: ",error, 255);
			free(error);
			exit(estatus);
		}
		i++;
	}
	estatus = ft_atoi(arg);
	if (estatus < -9223372036854775807)
	{
		if (g_vars.history)
				g_vars.history = ft_destroy_history(g_vars.history);
		error = ft_strjoin(arg, ": numeric argument required");
		estatus = ft_put_err("exit: ",error, 255);
		free(error);
		exit(estatus);
	}
	return (estatus);
}

int	ft_exit(t_args *args)
{
	int		i;

	if (args == NULL)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	ft_putendl_fd("exit", 1);
	i = ft_check_exit(args->value);
	if (args->next != NULL)
		return (ft_put_err("exit", ": too many arguments", 1));
	if (g_vars.history)
		g_vars.history = ft_destroy_history(g_vars.history);
	exit(i);
}
