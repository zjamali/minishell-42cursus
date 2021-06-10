/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chars_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:37:47 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 21:12:04 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_char_list	*ft_free_char_list(t_char_list *char_list)
{
	t_char_list	*tmp;

	tmp = NULL;
	while (char_list)
	{
		tmp = char_list;
		char_list = char_list->next;
		free(tmp);
		tmp = NULL;
	}
	return (NULL);
}

int	get_char_list_lenght(t_char_list *char_list)
{
	int	i;

	i = 0;
	if (char_list == NULL)
		return (i);
	else
	{
		while (char_list)
		{
			i++;
			char_list = char_list->next;
		}
		return (i);
	}
}

void	ft_delete_char_list(t_char_list *char_list)
{
	t_char_list	*tmp;
	t_char_list	*tmp1;

	tmp = char_list;
	while (tmp)
	{
		tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1);
		tmp1 = NULL;
	}
}

void	ft_print_char_list(t_char_list *chars_list)
{
	t_char_list	*tmp;

	tmp = chars_list;
	while (tmp)
	{
		if (tmp->value != 0)
			ft_putchar_fd(tmp->value, 1);
		tmp = tmp->next;
	}
}

t_char_list	*ft_copy_char_list(t_char_list *char_list)
{
	int			len;
	t_char_list	*tmp;
	t_char_list	*copy;
	t_char_list	*origin;

	origin = char_list;
	copy = NULL;
	len = get_char_list_lenght(origin);
	if (len == 0)
		return (NULL);
	else
	{
		copy = (t_char_list *)malloc(sizeof(t_char_list));
		tmp = copy;
		while (len > 0)
		{
			len = ft_copy_char_value(len, &origin, &tmp);
			if (len > 0)
			{
				tmp->next = (t_char_list *)malloc(sizeof(t_char_list));
				tmp = tmp->next;
			}
		}
		return (copy);
	}
}
