/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chars_list_rest.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:43:34 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:44:14 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

char	*create_line_from_chars_list(t_char_list *char_list)
{
	t_char_list	*tmp;
	char		*line;
	int			len;
	int			i;

	i = 0;
	len = char_list->len;
	tmp = char_list;
	line = (char *)malloc(len + 1);
	if (!line)
	{
		ft_putstr_fd("line allocation problem", 1);
		return (NULL);
	}
	ft_bzero(line, len + 1);
	while (tmp)
	{
		line[i] = tmp->value;
		tmp = tmp->next;
		i++;
	}
	line[len] = '\0';
	return (line);
}

int	ft_copy_char_value(int len, t_char_list **origin, t_char_list **tmp)
{
	(*tmp)->len = (*origin)->len;
	(*tmp)->value = (*origin)->value;
	(*tmp)->next = NULL;
	len--;
	*origin = (*origin)->next;
	return (len);
}

t_lines_list	*push_curr_node_to_list(t_lines_list **list,
				t_lines_list *current, int history)
{
	t_lines_list	*tmp_list;

	tmp_list = *list;
	if (current)
	{
		if ((*list)->prev != NULL)
		{
			while ((*list)->prev != NULL)
			{
				*list = (*list)->prev;
			}
		}
		current->history = history;
		current->next = (*list);
		current->index = (*list)->index + 1;
		current->prev = NULL;
		(*list)->prev = current;
		(*list) = current;
	}
	return (*list);
}

void	ft_add_to_char_list_non_empty_node(char c, t_char_list **chars_list)
{
	t_char_list	*tmp;

	tmp = NULL;
	tmp = *chars_list;
	while (tmp->next)
	{
		ft_putchar_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	ft_putchar_fd(tmp->value, 1);
	tmp->next = (t_char_list *)malloc(sizeof(t_char_list));
	tmp->next->next = NULL;
	tmp->next->value = c;
	tmp->next->len = tmp->len + 1;
	(*chars_list)->len = tmp->next->len;
	ft_putchar_fd(c, 1);
}

void	ft_add_to_char_list(t_readline *readline, char c,
		t_char_list **chars_list)
{
	t_char_list	*tmp;

	tmp = NULL;
	ft_move_cursor_and_clear(readline->cursor);
	if (*chars_list == NULL || ((*chars_list) && (*chars_list)->value == 0))
	{
		ft_putchar_fd(c, 1);
		if ((*chars_list))
			free(*chars_list);
		*chars_list = (t_char_list *)malloc(sizeof(t_char_list));
		(*chars_list)->value = c;
		(*chars_list)->len = 1;
		(*chars_list)->next = NULL;
	}
	else
	{
		ft_add_to_char_list_non_empty_node(c, chars_list);
	}
}
