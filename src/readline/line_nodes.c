/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:32:35 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:58:51 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_lines_list	*ft_create_line_node(void)
{
	t_lines_list	*ret;

	ret = (t_lines_list *)malloc(sizeof(t_lines_list));
	if (!ret)
		return (NULL);
	ret->char_list = NULL;
	ret->origin_char_list = NULL;
	ret->up_or_down = false;
	ret->next = NULL;
	ret->prev = NULL;
	ret->value = 0;
	ret->history = 0;
	return (ret);
}

t_lines_list	*ft_destory_node(t_lines_list *node)
{
	if (node->char_list)
		ft_delete_char_list(node->char_list);
	node->char_list = NULL;
	if (node->origin_char_list)
		ft_delete_char_list(node->origin_char_list);
	node->origin_char_list = NULL;
	node->next = NULL;
	node->prev = NULL;
	free(node);
	node = NULL;
	return (NULL);
}

t_lines_list	*ft_delete_node_from_list(t_lines_list *current)
{
	t_lines_list	*next_line;

	next_line = NULL;
	if (current == NULL)
		return (NULL);
	if (current->next)
		current->next->prev = current->prev;
	if (current->next)
		next_line = current->next;
	current = ft_destory_node(current);
	return (next_line);
}
