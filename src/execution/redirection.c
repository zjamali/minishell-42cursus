/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 16:39:20 by mbari             #+#    #+#             */
/*   Updated: 2021/04/12 17:29:22 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int		ft_great_than(int *fd, t_redirection *redirect)
{
	//int fd;

	if (((*fd) = open(redirect->file_name, O_CREAT | O_TRUNC | O_RDONLY | O_WRONLY, 0644)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2((*fd), STDOUT_FILENO);
	close((*fd));
	return (1);
}

int ft_less_than(int *fd, t_redirection *redirect)
{
	if (((*fd) = open(redirect->file_name, O_RDONLY)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2((*fd), STDIN_FILENO);
	close((*fd));
	return (1);
}

int ft_double_great(int *fd, t_redirection *redirect)
{
	if (((*fd) = open(redirect->file_name, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2((*fd), STDOUT_FILENO);
	close((*fd));
	return (1);
}

int	ft_redirection(t_mini *mini, t_redirection *redirect)
{
	while (redirect != NULL)
	{
		if (redirect->type == RE_GREAT)
			ft_great_than(&mini->red_fd[1], redirect);
		else if (redirect->type == RE_LESS)
			ft_less_than(&mini->red_fd[0], redirect);	
		else if (redirect->type == RE_DOUBLE_GREAT)
			ft_double_great(&mini->red_fd[1], redirect);
		redirect = redirect->next;
	}
	return (1);
}
