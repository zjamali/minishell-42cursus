/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 16:55:58 by mbari             #+#    #+#             */
/*   Updated: 2021/03/27 16:00:57 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/execution.h"

t_env     *ft_create_node(char *name, char *value)
{
    t_env *new;

    new = malloc(sizeof(t_env));
    if (new)
    {
        new->name = name;
        new->value = value;
        new->next = NULL;
    }
    return (new);
}

void    ft_add_to_list(t_env **head, t_env *newnode)
{
    t_env *list;
    if (!head || !newnode)
        return ;
    if (*head)
    {
        list = *head;
        while (list && list->next)
            list = list->next;
        list->next = newnode;
    }
    else
        *head = newnode;
}

void	ft_delete_from_list(t_env **head, char *name)
{
	t_env *prev;
	t_env *temp;

	temp = *head;
	if (*head == NULL)
		return ;
	
	if (!(ft_strcmp(temp->name, name)))
	{
		*head = temp->next;
		free(temp);
		return ;
	}

	while (temp != NULL && ft_strcmp(temp->name, name) != 0)
	{
		prev = temp;
		temp = temp->next;
	}
	if (temp == NULL)
		return ;
	else
	{
		prev->next = temp->next;
		free(temp);
	}
	
}

t_env	*ft_search_in_list(t_env **head, char *name)
{
	t_env *temp;

	temp = *head;
	if (temp == NULL)
		return (NULL);
	while (temp != NULL && ft_strcmp(temp->name, name) != 0)
		temp = temp->next;
	return (temp);
}

void	ft_replaceit(t_env **head, char *name, char *value)
{
	t_env *temp;
	
	temp = ft_search_in_list(head, name);
	if (temp != NULL)
		temp->value = value;
}