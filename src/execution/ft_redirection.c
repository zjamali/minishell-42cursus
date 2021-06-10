/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 16:39:20 by mbari             #+#    #+#             */
/*   Updated: 2021/06/06 09:56:59 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int	ft_great_than(int *fd, t_redirection *redirect)
{
	*fd = open(redirect->file_name, O_CREAT | O_TRUNC
			| O_RDONLY | O_WRONLY, 0644);
	if ((*fd) < 0)
		return (ft_file_err(redirect));
	dup2((*fd), STDOUT_FILENO);
	close((*fd));
	return (0);
}

int	ft_less_than(int *fd, t_redirection *redirect)
{
	*fd = open(redirect->file_name, O_RDONLY);
	if (*fd < 0)
		return (ft_file_err(redirect));
	dup2((*fd), STDIN_FILENO);
	close((*fd));
	return (0);
}

int	ft_double_great(int *fd, t_redirection *redirect)
{
	*fd = open(redirect->file_name, O_CREAT | O_RDONLY
			| O_WRONLY | O_APPEND, 0644);
	if (*fd < 0)
		return (ft_file_err(redirect));
	dup2((*fd), STDOUT_FILENO);
	close((*fd));
	return (0);
}

int	ft_file_err(t_redirection *redirect)
{
	struct stat		buf;

	if (stat(redirect->file_name, &buf) == 0)
	{
		if (buf.st_mode & S_IFDIR)
			return (ft_put_err(redirect->file_name, ": Is a directory", 1));
		else if ((buf.st_mode & S_IXUSR) == 0)
			return (ft_put_err(redirect->file_name, ": Permission denied", 1));
	}
	return (ft_put_err(redirect->file_name, ": No such file or directory", 1));
}

int	ft_redirection(t_mini *mini, t_redirection *redirect)
{
	while (redirect != NULL)
	{
		if (redirect->type == RE_GREAT)
		{
			if (ft_great_than(&mini->red_fd[1], redirect))
				return (1);
		}
		else if (redirect->type == RE_LESS)
		{
			if (ft_less_than(&mini->red_fd[0], redirect))
				return (1);
		}
		else if (redirect->type == RE_DOUBLE_GREAT)
		{
			if (ft_double_great(&mini->red_fd[1], redirect))
				return (1);
		}
		redirect = redirect->next;
	}
	return (0);
}
