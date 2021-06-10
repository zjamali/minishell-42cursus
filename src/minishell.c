/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 17:59:14 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:08:30 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/execution.h"

void	ft_tokinization_and_parsing(char **line,
		t_pipe_line **current_pipe_line, int *status)
{
	t_token		*tokens_list;

	tokens_list = NULL;
	if (*line)
	{
		tokens_list = ft_lexer(*line);
		free(*line);
		*line = NULL;
	}
	if (tokens_list)
	{
		g_vars.cmd = ft_parser(tokens_list, status);
		tokens_list = NULL;
	}
	if (g_vars.cmd)
		*current_pipe_line = g_vars.cmd->childs;
}

int	ft_expande_and_execute_pipe_line(t_pipe_line *current_pipe_line,
		char **last_arg_exit_sts, int *status, t_env **env_list)
{
	if (last_arg_exit_sts[0])
		free(last_arg_exit_sts[0]);
	last_arg_exit_sts[0] = ft_int_to_string(*status);
	ft_expanding(current_pipe_line, env_list, last_arg_exit_sts);
	current_pipe_line->child = ft_delete_emty_simple_cmds(&current_pipe_line);
	if (current_pipe_line->child)
	{
		if (last_arg_exit_sts[1])
			free(last_arg_exit_sts[1]);
		last_arg_exit_sts[1] = get_last_argument_or_command(current_pipe_line);
		*status = ft_execute(current_pipe_line, env_list);
	}
	return (*status);
}

void	ft_init_minishell(char	**last_arg_exit_status, char **line,
		t_pipe_line **current_pipe_line, t_env **env_list)
{
	g_vars.cmd = NULL;
	*current_pipe_line = NULL;
	last_arg_exit_status[0] = ft_strdup("0");
	*line = NULL;
	*env_list = NULL;
}

void	ft_destroy_main_ast(void)
{
	if (g_vars.cmd)
	{
		ft_destroy_ast(g_vars.cmd);
		ft_putstr_fd(RESET, ft_strlen(RESET));
		g_vars.cmd = NULL;
	}
}

void	ft_minishell(char **env)
{
	t_pipe_line	*currnt_pipe_line;
	t_env		*env_list;
	char		*line;
	int			status;
	static char	*last_arg_exit_sttus[2];

	status = 0;
	ft_init_minishell(last_arg_exit_sttus, &line, &currnt_pipe_line, &env_list);
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, ft_signal_handler);
	ft_init_env(&env_list, env);
	while (1337)
	{
		show_prompt();
		micro_read_line(&line, &status);
		ft_tokinization_and_parsing(&line, &currnt_pipe_line, &status);
		while (currnt_pipe_line)
		{
			status = ft_expande_and_execute_pipe_line(currnt_pipe_line,
					last_arg_exit_sttus, &status, &env_list);
			currnt_pipe_line = currnt_pipe_line->next;
		}
		ft_destroy_main_ast();
	}
	ft_destroy_env_list(env_list);
}
