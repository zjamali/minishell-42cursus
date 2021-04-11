/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/10 16:39:20 by mbari             #+#    #+#             */
/*   Updated: 2021/04/11 17:40:08 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int		ft_great_than(t_redirection *redirect)
{
	int fd;

	if ((fd = open(redirect->file_name, O_CREAT | O_TRUNC | O_RDONLY | O_WRONLY, 0644)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int ft_less_than(t_redirection *redirect)
{
	int fd;

	if ((fd = open(redirect->file_name, O_RDONLY)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}

int ft_double_great(t_redirection *redirect)
{
	int fd;

	if ((fd = open(redirect->file_name, O_CREAT | O_RDONLY | O_WRONLY | O_APPEND, 0644)) < 0)
		return (ft_put_err(redirect->file_name, ": No such file or directory", 0));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (1);
}

int	ft_redirection(t_redirection *redirect)
{
	while (redirect != NULL)
	{
		if (redirect->type == RE_GREAT)
			ft_great_than(redirect);
		else if (redirect->type == RE_LESS)
			ft_less_than(redirect);	
		else if (redirect->type == RE_DOUBLE_GREAT)
			ft_double_great(redirect);
		redirect = redirect->next;
	}
	return (1);
}
