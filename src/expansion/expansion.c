/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 14:41:59 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:08:58 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

void	ft_expande_simple_cmd(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_args			*args;
	t_args			*to_free;

	to_free = NULL;
	args = NULL;
	ft_expand_arguments(cmd, env, last_env);
	if ((*cmd)->command)
		ft_expand_command(cmd, env, last_env);
	ft_expande_redirection(cmd, env, last_env);
	args = (*cmd)->args;
	if (args)
		ft_delete_emty_args_nodes(&(*cmd)->args);
	if (!(*cmd)->command && (*cmd)->inside_quotes == 0)
		ft_handle_empty_command(cmd);
	ft_return_spaces(cmd);
}

void	ft_expanding(t_pipe_line *pipe_line, t_env **env, char **last_env)
{
	t_simple_cmd	*current_cmd;
	t_simple_cmd	*head_cmd;

	current_cmd = NULL;
	head_cmd = NULL;
	ft_putstr_fd(RED, 1);
	head_cmd = pipe_line->child;
	while (head_cmd)
	{
		current_cmd = head_cmd;
		ft_expande_simple_cmd(&current_cmd, env, last_env);
		head_cmd = head_cmd->next;
	}
}
