/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:32:24 by mbari             #+#    #+#             */
/*   Updated: 2021/06/03 16:11:46 by mbari            ###   ########.fr       */
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

int	ft_check_errors(char *dir)
{
	char	*error;
	char	*join;
	char	*buff;

	buff = NULL;
	join = NULL;
	error = NULL;
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
	return (0);
}

int	ft_change_dir(t_env **head, char *dir)
{
	t_env	*temp;

	temp = ft_search_in_list(head, "PWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("PWD", ""));
	temp = ft_search_in_list(head, "OLDPWD");
	if (temp == NULL)
		ft_add_to_list(head, ft_create_node("OLDPWD", ""));
	if (ft_check_errors(dir))
		return (1);
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
	return (ft_change_dir(head, dir));
}
