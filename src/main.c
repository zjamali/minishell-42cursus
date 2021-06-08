/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/08 22:00:21 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/execution.h"

char	*return_last_argument(char	**split)
{
	char	*last_args;

	last_args = ft_strdup(split[0]);
	free(split[0]);
	free(split);
	return (last_args);
}

char	*ft_return_last_argmnt(char *argument_value)
{
	char	**split;

	split = ft_split(argument_value, '=');
	if (split[0] == NULL)
		split[0] = ft_strdup("=");
	else
	{
		if (split[0][0] != argument_value[0])
		{
			free(split[0]);
			split[0] = ft_strdup(argument_value);
		}
	}
	free(split[1]);
	if (split[0])
		return (return_last_argument(split));
	else
	{
		free(split);
		return (NULL);
	}
}

char	*get_last_argument_or_command(t_pipe_line *current_pipe_line)
{
	t_args	*args;

	if (current_pipe_line->child->next != NULL)
		return (NULL);
	else
	{
		args = current_pipe_line->child->args;
		if (args == NULL)
		{
			if (current_pipe_line->child->command)
				return (ft_strdup(current_pipe_line->child->command));
		}
		else
		{
			while (args->next != NULL)
				args = args->next;
			if (current_pipe_line->child->command
				&&!ft_strcmp(current_pipe_line->child->command, "export"))
				return (ft_return_last_argmnt(args->value));
			else
			{
				if (args->value)
					return (ft_strdup(args->value));
				else
				{
					if (current_pipe_line->child->command)
						return (ft_strdup(current_pipe_line->child->command));
				}
			}
		}
	}
	return (NULL);
}

void	show_prompt(void)
{
	write(1, GREEN, ft_strlen(GREEN));
	write(1, "minishell$ ", strlen("minishell$ "));
	write(1, RESET, ft_strlen(RESET));
}

t_simple_cmd	*ft_delete_emty_simple_cmds(t_pipe_line **pipe_line)
{
	t_simple_cmd	*head;
	t_simple_cmd	*prev;
	t_simple_cmd	*temp;

	prev = NULL;
	head = (*pipe_line)->child;
	temp = head;
	while (temp != NULL && (!temp->command && temp->inside_quotes == 0)
		&& temp->args == NULL && temp->redirections == NULL)
	{
		head = temp->next;
		ft_destroy_simple(temp);
		temp = head;
	}
	while (temp != NULL)
	{
		while (temp != NULL && (temp->inside_quotes != 0
				|| (temp->command && temp->inside_quotes == 0)))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return (head);
		if (prev)
		{
			prev->next = temp->next;
			ft_destroy_simple(temp);
			temp = prev->next;
		}
		else
			temp = temp->next;
	}
	return (head);
}

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

void	ft_destroy_main_ast(void)
{
	if (g_vars.cmd)
	{
		ft_destroy_ast(g_vars.cmd);
		ft_putstr_fd(RESET, ft_strlen(RESET));
		g_vars.cmd = NULL;
	}
}
void	ft_init_minishell(char	**last_arg_exit_status, char	**line,
		t_pipe_line **current_pipe_line, t_env **env_list)
{
	g_vars.cmd = NULL;
	*current_pipe_line = NULL;
	last_arg_exit_status[0] = ft_strdup("0");
	*line = NULL;
	*env_list = NULL;
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

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	ft_minishell(env);
	return (0);
}
