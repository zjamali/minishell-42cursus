/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 12:45:20 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:44:19 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_char_list	*init_character_list(void)
{
	t_char_list	*tmp;

	tmp = (t_char_list *)malloc(sizeof(t_char_list));
	tmp->len = 0;
	tmp->next = NULL;
	tmp->value = 0;
	return (tmp);
}

t_lines_list	*ft_up_in_history(t_readline *readline, t_lines_list **history)
{
	ft_move_cursor_and_clear(readline->cursor);
	if ((*history)->next)
	{
		*history = (*history)->next;
		if (*history)
		{
			(*history)->up_or_down = true;
			ft_print_char_list((*history)->char_list);
		}
	}
	else
	{
		if (history)
		{
			(*history)->up_or_down = true;
			if ((*history)->char_list)
				ft_print_char_list((*history)->char_list);
		}
	}
	return (*history);
}

t_lines_list	*ft_down_in_history(t_readline *readline,
				t_lines_list **history)
{
	ft_move_cursor_and_clear(readline->cursor);
	if ((*history))
	{
		if ((*history)->prev != NULL)
		{
			*history = (*history)->prev;
			(*history)->up_or_down = true;
		}
		ft_print_char_list((*history)->char_list);
	}
	return (*history);
}

char	*ft_return_line(t_readline readline)
{
	char	*line;

	line = NULL;
	if (readline.line)
	{
		line = ft_strdup(readline.line);
		free(readline.line);
		readline.line = NULL;
	}
	return (line);
}

int	micro_read_line(char **line, int *status)
{
	t_readline		readline;
	struct termios	termios;
	struct termios	old_termios;

	ft_init_readline(&readline);
	set_terminal(&termios, &old_termios, readline.term_fd);
	ft_get_cursor_position(&readline.cursor.line_postion,
		&readline.cursor.col_position);
	*line = ft_get_input(readline, status, old_termios);
	reset_terminal(old_termios, readline.term_fd);
	close(readline.term_fd);
	return (1);
}
