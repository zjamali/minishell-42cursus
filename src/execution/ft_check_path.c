/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 19:58:33 by mbari             #+#    #+#             */
/*   Updated: 2021/06/12 13:59:59 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

int	ft_file_check(t_simple_cmd *cmd, t_env **head)
{
	struct stat		buf;

	if (stat(cmd->command, &buf) == -1)
		return (ft_put_err(cmd->command, ": No such file or directory", 127));
	else if (buf.st_mode & S_IFDIR)
		return (ft_put_err(cmd->command, ": Is a directory", 126));
	else if ((buf.st_mode & S_IXUSR) == 0)
		return (ft_put_err(cmd->command, ": Permission denied", 126));
	return (ft_exec(cmd, head));
}

char	*ft_join_path(char *path, char *cmd)
{
	char	*path_tmp;
	char	*full_path;

	path_tmp = ft_strjoin(path, "/");
	full_path = ft_strjoin(path_tmp, cmd);
	free(path_tmp);
	return (full_path);
}

int	ft_find_file(t_simple_cmd *cmd, t_env **head)
{
	t_env			*temp;
	char			**path;
	int				status;
	int				i;

	i = 0;
	status = -77;
	temp = ft_search_in_list(head, "PATH");
	if (temp == NULL)
		return (ft_file_check(cmd, head));
	path = ft_split(temp->value, ':');
	while (path[i] != NULL)
	{
		ft_exec_file(cmd, head, path[i], &status);
		free(path[i]);
		i++;
	}
	free(path);
	return (status);
}

int	ft_check_path(t_simple_cmd *cmd, t_env **head)
{
	int				status;

	if (ft_strchr(cmd->command, '/') || cmd->command[0] == '.')
		return (ft_file_check(cmd, head));
	else
	{
		status = ft_find_file(cmd, head);
		if (status != -77)
			return (status);
		return (ft_put_err(cmd->command, ": command not found", 127));
	}
	return (0);
}
