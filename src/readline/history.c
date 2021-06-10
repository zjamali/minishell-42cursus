/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:34:27 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:59:07 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_lines_list	*ft_destroy_history(t_lines_list *history)
{
	t_lines_list	*last_node;
	t_lines_list	*tmp;

	tmp = NULL;
	last_node = history;
	if (last_node->next != NULL)
	{
		while (last_node->next != NULL)
			last_node = last_node->next;
	}
	tmp = last_node;
	while (tmp)
	{
		last_node = tmp;
		tmp = tmp->prev;
		last_node = ft_destory_node(last_node);
	}
	return (NULL);
}

t_lines_list	*ft_insert_node_to_history(t_lines_list *list,
				t_lines_list *current, int history)
{
	if (!list)
	{
		current->history = history;
		current->index = 1;
		list = current;
		list->next = NULL;
		list->prev = NULL;
	}
	else
		list = push_curr_node_to_list(&list, current, history);
	return (list);
}

int	add_curr_node_to_history(t_readline *readline,
				  t_lines_list *current, t_lines_list **history)
{
	t_lines_list	*new_line_node;

	new_line_node = NULL;
	new_line_node = ft_create_line_node();
	new_line_node->char_list = ft_copy_char_list(current->char_list);
	new_line_node->origin_char_list = ft_copy_char_list(current->char_list);
	ft_delete_char_list(current->char_list);
	current->char_list = ft_copy_char_list(current->origin_char_list);
	*history = ft_delete_node_from_list(*history);
	if (new_line_node && new_line_node->char_list
		&& new_line_node->char_list->value != 0)
	{
		*history = ft_insert_node_to_history(*history, new_line_node, 0);
		readline->line = create_line_from_chars_list(new_line_node->char_list);
	}
	else
		ft_destory_node(new_line_node);
	ft_putstr_fd("\n", 1);
	return (0);
}

t_lines_list	*ft_init_history(long *character, int *newline_break)
{
	t_lines_list	*current;
	static bool		inisialize_history = false;

	current = ft_create_line_node();
	*character = 0;
	if (inisialize_history == false)
	{
		g_vars.history = NULL;
		inisialize_history = true;
	}
	current->next = g_vars.history;
	if (g_vars.history)
		g_vars.history->prev = current;
	g_vars.history = current;
	*newline_break = 1337;
	return (current);
}

t_lines_list	*ft_handle_history_buttons(long character, t_readline readline,
				t_lines_list *current)
{
	if (current && character == D_KEY_UP)
		current = ft_up_in_history(&readline, &current);
	else if (current && character == D_KEY_DOWN)
		current = ft_down_in_history(&readline, &current);
	return (current);
}
