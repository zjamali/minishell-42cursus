/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:39:48 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/12 16:07:25 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	get_charcter(t_readline *readline, long c, t_lines_list *current)
{
	ft_add_to_char_list(readline, c, &current->char_list);
}

void	ft_delete_last_charactere_from_line(t_lines_list **current,
		t_readline *readline)
{
	t_char_list	*tmp;
	int			len;
	int			i;

	i = 1;
	len = get_char_list_lenght((*current)->char_list);
	tmp = (*current)->char_list;
	if (len == 0)
		return ;
	while (len - 1 > i)
	{
		i++;
		tmp = tmp->next;
	}
	if (tmp->next)
	{
		free(tmp->next);
		tmp->next = NULL;
	}
	else if (tmp->next == NULL)
		tmp->value = 0;
	ft_move_cursor_and_clear(readline->cursor);
	if ((*current)->char_list)
		ft_print_char_list((*current)->char_list);
}

int	ft_handle_ctrl_c_input(t_lines_list **current, int *status)
{
	if (*current && (*current)->char_list == NULL
		&& (*current)->origin_char_list == NULL && (*current)->prev == NULL)
		g_vars.history = ft_delete_node_from_list((*current));
	else
	{
		ft_delete_char_list((*current)->char_list);
		(*current)->char_list = ft_copy_char_list((*current)->origin_char_list);
		g_vars.history = ft_delete_node_from_list(g_vars.history);
	}
	*status = 1;
	ft_putstr_fd("\n", 1);
	return (0);
}

void	ft_handle_ctrl_d_input(t_lines_list **current, int *status,
			t_readline readline, struct termios old_term)
{
	if (*current && ((*current)->char_list == NULL
			|| (*current)->char_list->value == 0))
	{
		if (g_vars.history)
			g_vars.history = ft_destroy_history(g_vars.history);
		if (g_vars.cmd)
			ft_destroy_ast(g_vars.cmd);
		reset_terminal(old_term, readline.term_fd);
		*status = 127;
		ft_putstr_fd("exit", 1);
		exit(*status);
	}
}

char	*ft_get_input(t_readline readline, int *status, struct termios old_term)
{
	long			character;
	t_lines_list	*current;
	int				newline_break;

	current = ft_init_history(&character, &newline_break);
	while (newline_break)
	{
		read(readline.term_fd, &character, 6);
		if (current && (character == D_KEY_DOWN || character == D_KEY_UP))
			current = ft_handle_history_buttons(character, readline, current);
		else if (character == D_KEY_ENTER)
			newline_break = add_curr_node_to_history(&readline,
					current, &g_vars.history);
		else if (character == D_KEY_BACKSPACE)
			ft_delete_last_charactere_from_line(&current, &readline);
		else if (character == D_KEY_CTRL_C)
			newline_break = ft_handle_ctrl_c_input(&current, status);
		else if (character == D_KEY_CTRL_D)
			ft_handle_ctrl_d_input(&current, status, readline, old_term);
		else if (ft_isprint(character))
			get_charcter(&readline, character, current);
		character = 0;
	}
	return (ft_return_line(readline));
}
