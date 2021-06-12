/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_config.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:29:06 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/12 16:06:59 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_move_cursor_and_clear(t_cursor cursor)
{
	char	*move_cursor;

	move_cursor = tgetstr("cm", NULL);
	ft_putstr_fd(tgoto(move_cursor, cursor.col_position - 1,
			cursor.line_postion - 1), 1);
	ft_putstr_fd(tgetstr("cd", NULL), 1);
}

void	ft_init_readline(t_readline *readline)
{
	int	load_term;

	readline->term_type = getenv("TERM");
	if (readline->term_type == NULL)
		readline->term_type = "xterm-256color";
	load_term = tgetent(NULL, readline->term_type);
	if (!load_term)
	{
		ft_putstr_fd("Could not access to the termcap database orterminal \
		type is not defined in termcap\n", 1);
	}
	readline->path = ttyname(1);
	readline->term_fd = open(readline->path, O_RDWR);
	if (!isatty(readline->term_fd))
		ft_putstr_fd("file descriptors not point to terminal\n", 1);
	readline->colums_count = tgetnum("co");
	readline->line_count = tgetnum("li");
	readline->cursor.col_position = 0;
	readline->cursor.line_postion = 0;
	readline->line = NULL;
}

void	set_terminal(struct termios *termios, struct termios *old, int fd)
{
	tcgetattr(fd, old);
	tcgetattr(fd, termios);
	termios->c_lflag &= ~(ECHO | ICANON | ISIG);
	tcsetattr(fd, TCSANOW, termios);
}

void	reset_terminal(struct termios old_termios, int fd)
{
	tcsetattr(fd, TCSANOW, &old_termios);
}

void	ft_get_cursor_position(int *x, int *y)
{
	static char	c[20];
	int			i;
	int			first;

	i = 0;
	first = 0;
	write(0, "\033[6n", 4);
	read(0, c, 19);
	while (c[i])
	{
		if (first == 0 && ft_isdigit(c[i]))
		{
			*x = ft_atoi(c + i);
			while (c[i] != ';')
				i++;
			first = 1;
		}
		else if (first == 1 && ft_isdigit(c[i]))
		{
			*y = ft_atoi(c + i);
			first = 2;
		}
		i++;
	}
}
