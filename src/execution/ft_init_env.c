/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 20:01:59 by mbari             #+#    #+#             */
/*   Updated: 2021/06/12 13:58:42 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

int	ft_count_list(t_env **head)
{
	int		i;
	t_env	*temp;

	i = 0;
	temp = *head;
	while (temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

void	ft_free_list(char **list)
{
	int		i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
}

char	**ft_args_to_arr(t_simple_cmd *cmd)
{
	t_args	*temp;
	char	**arg;
	int		i;

	temp = cmd->args;
	i = ft_count_args(&temp) + 1;
	arg = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	arg[i++] = ft_strdup(cmd->command);
	while (temp != NULL)
	{
		if (temp->value == NULL && temp->inside_quotes == 2)
			temp->value = ft_strdup("");
		arg[i] = ft_strdup(temp->value);
		i++;
		temp = temp->next;
	}
	arg[i] = NULL;
	return (arg);
}

void	ft_init_env(t_env **head, char **env)
{
	t_env	*newnode;
	char	**var;
	int		i;

	newnode = NULL;
	i = 0;
	while (env[i])
	{
		var = my_split(env[i]);
		newnode = ft_create_node(var[0], var[1]);
		if (!head)
			head = &newnode;
		else
			ft_add_to_list(head, newnode);
		free(var[0]);
		free(var[1]);
		free(var);
		i++;
	}
}
