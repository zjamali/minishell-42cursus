/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:32:24 by mbari             #+#    #+#             */
/*   Updated: 2021/05/23 17:32:43 by mbari            ###   ########.fr       */
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
	return (0);
}

int	ft_change_dir(t_args *args, t_env **head, char *dir)
{
	char	*buff;
	t_env	*temp;

	buff = NULL;
	if (!getcwd(buff, 100))
		ft_put_err(ft_strjoin("cd: error retrieving current directory: ",
				"getcwd: cannot access parent directories: "),
			strerror(errno), 1);
	temp = ft_search_in_list(head, "PWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("PWD", ""));
	temp = ft_search_in_list(head, "OLDPWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("OLDPWD", ""));
	if (chdir(dir) == -1)
		return (ft_put_err("cd: ", ft_strjoin(dir,
					ft_strjoin(": ", strerror(errno))), 1));
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
		dir = temp->value;
	}
	else
		dir = args->value;
	return (ft_change_dir(args, head, dir));
}

int	ft_check_exit(char *arg)
{
	int		i;
	long long	estatus;

	i = 0;
	if (arg[i] == '-')
			i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (ft_put_err("exit: ",
					ft_strjoin(arg, ": numeric argument required"), 255));
		i++;
	}
	estatus = ft_atoi(arg);
	if (estatus < 9223372036854775807)
		return (ft_put_err("exit: ", ft_strjoin(arg, ": numeric argument required"), 255));
	return (estatus);
}

int	ft_exit(t_args *args)
{
	if (args == NULL)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	if (args->next != NULL)
		return (ft_put_err("exit", ": too many arguments", 1));
	ft_putendl_fd("exit", 1);
	exit(ft_check_exit(args->value));
}
