/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_linked_lists_rest.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 19:50:02 by mbari             #+#    #+#             */
/*   Updated: 2021/06/06 09:59:30 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

void	ft_replaceit(t_env **head, char *name, char *value)
{
	t_env	*temp;

	temp = ft_search_in_list(head, name);
	if (temp != NULL)
	{
		free(temp->value);
		temp->value = ft_strdup(value);
	}
}

char	**ft_list_to_arr(t_env **head)
{
	t_env	*temp;
	char	**env;
	char	*tmp_name;
	int		i;

	temp = *head;
	i = ft_count_list(head);
	env = (char **)malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (temp != NULL)
	{
		tmp_name = ft_strjoin(temp->name, "=");
		env[i] = ft_strjoin(tmp_name, temp->value);
		free(tmp_name);
		i++;
		temp = temp->next;
	}
	env[i] = NULL;
	return (env);
}

t_env	*ft_copy_list(t_env **head)
{
	t_env	*temp;
	t_env	*newnode;
	t_env	*copy;

	temp = *head;
	copy = NULL;
	if (*head == NULL)
		return (NULL);
	while (temp != NULL)
	{
		newnode = ft_create_node(temp->name, temp->value);
		if (copy == NULL)
			copy = newnode;
		else
			ft_add_to_list(&copy, newnode);
		temp = temp->next;
	}
	return (copy);
}

void	ft_swap_nodes(t_env	*temp)
{
	char	*tmp;

	if (strcmp(temp->name, temp->next->name) > 0)
	{
		tmp = ft_strdup(temp->next->name);
		free(temp->next->name);
		temp->next->name = ft_strdup(temp->name);
		free(temp->name);
		temp->name = tmp;
		tmp = ft_strdup(temp->next->value);
		free(temp->next->value);
		temp->next->value = ft_strdup(temp->value);
		free(temp->value);
		temp->value = tmp;
	}
}

t_env	*ft_sort_list(t_env **head)
{
	t_env	*temp;
	t_env	*sort_list;
	int		i;

	sort_list = ft_copy_list(head);
	i = ft_count_list(&sort_list);
	temp = sort_list;
	while (i > 0)
	{
		temp = sort_list;
		while (temp->next != NULL)
		{
			ft_swap_nodes(temp);
			temp = temp->next;
		}
		i--;
	}
	return (sort_list);
}
