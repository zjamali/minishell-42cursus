/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 16:55:58 by mbari             #+#    #+#             */
/*   Updated: 2021/05/31 11:33:39 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

t_env     *ft_create_node(char *name, char *value)
{
    t_env *new;
    new = malloc(sizeof(t_env));
    if (new)
    {
        new->name = ft_strdup(name);
        new->value = ft_strdup(value);
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
		free(temp->name);
		free(temp->value);
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
		free(temp->name);
		free(temp->value);
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
	{
		free(temp->value);
		temp->value = ft_strdup(value);
	}
}

int		ft_count_list(t_env **head)
{
	int i;
	t_env *temp;

	i = 0;
	temp = *head;
	while (temp != NULL)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char **ft_list_to_arr(t_env **head)
{
	t_env *temp;
	char **env;
	char *tmp_name;
	int i;

	temp = *head;
	i = ft_count_list(head);
	env = (char **)malloc(sizeof(char *) * i + 1);
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

t_env *ft_copy_list(t_env **head)
{
	t_env *temp;
	t_env *newnode;
	t_env *copy;

	temp = *head;
	copy = NULL;
	if (*head == NULL)
		return (NULL);
	while (temp != NULL)
	{
		newnode = ft_create_node(temp->name, temp->value);
		ft_add_to_list(&copy, newnode);
		temp = temp->next;
	}
	return (copy);
}

t_env	*ft_sort_list(t_env **head)
{
	t_env *temp;
	t_env *sort_list;
	char	*tmp;
	int i;
	
	sort_list = ft_copy_list(head);
	i = ft_count_list(&sort_list);
	temp = sort_list;	
	while (i > 0)
	{
		temp = sort_list;
		while(temp->next != NULL)
		{
			if(strcmp(temp->name,temp->next->name) > 0)
			{
				/*
					ft_strcpy(tmp,temp->name);
					ft_strcpy(temp->name,temp->next->name);
					ft_strcpy(temp->next->name,tmp);
					temp->name = temp->next->name;
					temp->next->name = tmp;
					ft_strcpy(tmp,temp->value);
					temp->value = temp->next->value;
					temp->next->value = temp->value;
					ft_strcpy(temp->value,temp->next->value);
					ft_putendl_fd(tmp, 1);
					ft_strcpy(temp->next->value,tmp);
				*/
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
			temp = temp->next;
		}
		i--;
	}
	return (sort_list);
}