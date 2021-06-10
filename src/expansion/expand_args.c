/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:19:16 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 19:22:39 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

void	ft_handle_arg_expanding(t_args **args)
{
	char	**splited;
	t_args	*tmp;
	t_args	*next;
	int		i;

	i = 0;
	splited = ft_split((*args)->value, ' ');
	tmp = *args;
	next = (*args)->next;
	free((*args)->value);
	while (splited[i])
	{
		tmp->value = splited[i];
		tmp->inside_quotes = 0;
		tmp->next = NULL;
		if (splited[i + 1])
		{
			tmp->next = (t_args *)malloc(sizeof(t_args));
			tmp = tmp->next;
		}
		i++;
	}
	tmp->next = next;
	free(splited);
}

t_args	*ft_expand_argument(char *befor_expand_arg, char	*after_expand_arg,
		t_args *args)
{
	t_args	*next_args;
	char	*space;

	if (args->value)
		after_expand_arg = ft_strdup(args->value);
	if (args->inside_quotes == 0 && after_expand_arg
		&& ft_strcmp(befor_expand_arg, after_expand_arg)
		&& ft_strchr(after_expand_arg, ' ')
		&& ft_strchr(befor_expand_arg, '$'))
	{
		next_args = args->next;
		space = ft_strchr(after_expand_arg, ' ');
		if (++space)
			ft_handle_arg_expanding(&args);
		args = next_args;
	}
	else
		args = args->next;
	free(after_expand_arg);
	return (args);
}

void	ft_expand_arguments(t_simple_cmd **cmd, t_env **env, char **last_env)
{
	t_args			*args;
	char			*befor_expand_arg;
	char			*after_expand_arg;

	args = (*cmd)->args;
	befor_expand_arg = NULL;
	after_expand_arg = NULL;
	while (args)
	{
		befor_expand_arg = ft_strdup(args->value);
		ft_repalce_space_by_tab(cmd, 0);
		args->inside_quotes = check_exiting_of_qoutes(args->value);
		ft_expande_word(&args->value, env, last_env, 0);
		args = ft_expand_argument(befor_expand_arg, after_expand_arg, args);
		free(befor_expand_arg);
		befor_expand_arg = NULL;
		after_expand_arg = NULL;
	}
}
