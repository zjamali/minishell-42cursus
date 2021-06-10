/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_continue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:10:25 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:09:50 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

void	ft_delete_emty_args_nodes(t_args **args)
{
	t_args	*temp;
	t_args	*prev;

	temp = *args;
	while (*args && ((*args)->inside_quotes == 0 && !(*args)->value))
	{
		temp = *args;
		*args = (*args)->next;
		free(temp);
	}
	temp = *args;
	while (temp)
	{
		while (temp && (temp->inside_quotes != 0
				|| (temp->value && temp->inside_quotes == 0)))
		{
			prev = temp;
			temp = temp->next;
		}
		if (temp == NULL)
			return ;
		prev->next = temp->next;
		free(temp);
		temp = prev->next;
	}
}

void	ft_return_spaces_to_arguments(t_simple_cmd **cmd)
{
	t_args	*args;
	int		i;

	i = 0;
	args = (*cmd)->args;
	while (args)
	{
		i = 0;
		if (args->value)
		{
			while (args->value[i])
			{
				if (args->value[i] == '\t')
					args->value[i] = ' ';
				i++;
			}
		}
		args = args->next;
	}
}

void	ft_return_spaces(t_simple_cmd **cmd)
{
	int		i;

	i = 0;
	if ((*cmd)->command)
	{
		while ((*cmd)->command[i] != '\0')
		{
			if ((*cmd)->command[i] == '\t')
				(*cmd)->command[i] = ' ';
			i++;
		}
	}
	ft_return_spaces_to_arguments(cmd);
}

void	ft_assign_value_to_arg(char	**splited, int i, t_args **new_args)
{
	(*new_args)->value = splited[i];
	(*new_args)->inside_quotes = 0;
	(*new_args)->next = NULL;
	if (splited[i + 1])
	{
		(*new_args)->next = (t_args *)malloc(sizeof(t_args));
		*new_args = (*new_args)->next;
	}
}

int	check_end_of_string(char str, int inside_quotes)
{
	if (inside_quotes == 0)
	{
		if (!str)
			return (1);
	}
	else
	{
		if (str == '\"')
			return (1);
	}
	return (0);
}
