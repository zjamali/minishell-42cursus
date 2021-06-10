/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:17:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 19:19:52 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

t_simple_cmd	*ft_handle_cmd_expanding(t_simple_cmd **cmd)
{
	char	**splited;
	int		i;
	t_args	*new_args;
	t_args	*tmp;

	i = 1;
	splited = ft_split((*cmd)->command, ' ');
	free((*cmd)->command);
	(*cmd)->command = splited[0];
	new_args = (t_args *)malloc(sizeof(t_args));
	tmp = new_args;
	while (splited[i])
	{
		ft_assign_value_to_arg(splited, i, &new_args);
		i++;
	}
	if (i > 1)
	{
		new_args->next = (*cmd)->args;
		(*cmd)->args = tmp;
	}
	else
		free(new_args);
	free(splited);
	return (*cmd);
}

void	ft_expand_command(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	char	*befor_expand_cmd;
	char	*after_expand_cmd;
	char	*space;

	space = NULL;
	befor_expand_cmd = NULL;
	after_expand_cmd = NULL;
	befor_expand_cmd = ft_strdup((*cmd)->command);
	ft_repalce_space_by_tab(cmd, 1);
	(*cmd)->inside_quotes = check_exiting_of_qoutes(((*cmd)->command));
	ft_expande_word(&((*cmd)->command), env, last_env, 0);
	if ((*cmd)->command)
		after_expand_cmd = ft_strdup((*cmd)->command);
	if ((*cmd)->inside_quotes == 0 && after_expand_cmd
		&& ft_strcmp(befor_expand_cmd, after_expand_cmd)
		&& ft_strchr(after_expand_cmd, ' ')
		&& ft_strchr(befor_expand_cmd, '$'))
	{
		space = ft_strchr(after_expand_cmd, ' ');
		if (++space)
			*cmd = ft_handle_cmd_expanding(cmd);
	}
	free(befor_expand_cmd);
	free(after_expand_cmd);
}

void	ft_handle_empty_command(t_simple_cmd **cmd)
{
	t_args	*args;
	t_args	*to_free;

	args = (*cmd)->args;
	if (args)
	{
		if (args->value)
		{
			(*cmd)->command = ft_strdup(args->value);
			free(args->value);
		}
		(*cmd)->inside_quotes = args->inside_quotes;
		if (args->next != NULL)
		{
			to_free = args;
			(*cmd)->args = args->next;
			free(to_free);
			to_free = NULL;
		}
		else
		{
			free((*cmd)->args);
			(*cmd)->args = NULL;
		}
	}
}
