/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/24 12:45:20 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/28 21:31:15 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../headers/minishell.h"
t_lines_list *ft_destory_node(t_lines_list *node)
{
	if (node->char_list)
		ft_delete_char_list(node->char_list);
	node->char_list = NULL;
	if (node->origin_char_list)
		ft_delete_char_list(node->origin_char_list);
	node->origin_char_list = NULL;
	node->next = NULL;
	node->prev = NULL;
	free(node);  //////////////
	node = NULL;
	return NULL;
}

t_lines_list *ft_destroy_history(t_lines_list *history)
{
	t_lines_list *last_node;
	t_lines_list *tmp;
	

	tmp = NULL;
	last_node = history;
	if (last_node->next != NULL)
	{
		while (last_node->next != NULL)
		{
			last_node = last_node->next;
		}
	}
	tmp = last_node;
	while (tmp)
	{
		last_node = tmp;
		//if (tmp)
			tmp = tmp->prev;
		//if (last_node)
		last_node = ft_destory_node(last_node);
	}
	return NULL;
}
void ft_delete_char_list(t_char_list *char_list)
{
	t_char_list *tmp;
	t_char_list *tmp1;

	tmp = char_list;
	
	while (tmp)
	{
		tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1);
		tmp1 = NULL;
	}
}


void ft_print_char_list(t_char_list *chars_list)
{
	t_char_list *tmp;

	tmp = chars_list;

	//while (tmp->next)
	//{
	//	ft_putchar_fd(tmp->value, 1);
	//	tmp = tmp->next;
	//}
	//if (tmp)
	//	ft_putchar_fd(tmp->value, 1);
	while (tmp)
	{
		
		if (tmp->value != 0)
			ft_putchar_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	
}

void ft_print_lines_list(t_lines_list *lines_list)
{
	while (lines_list)
	{
		ft_putstr_fd("\nline-> ", 1);
		//ft_putstr_fd(lines_list->value, 1);
		if (lines_list->char_list)
			ft_print_char_list(lines_list->char_list);
		else
			ft_putstr_fd("ana khawya",1);
		ft_putstr_fd("\thistory: ",1);
		ft_putnbr_fd(lines_list->history,1);
		ft_putstr_fd("\n", 1);
		lines_list = lines_list->next;
	}
}

void ft_print_lines_list_all(t_lines_list *lines_list)
{
	t_lines_list *tmp ;

	tmp = lines_list;
	
	if (tmp->prev != NULL)
	{
		while (tmp->prev != NULL)
			tmp = tmp->prev;
	}
	if (tmp)
		ft_print_lines_list(tmp);
}


void ft_get_cursor_position(int *x, int *y)
{
	int i = 0;
	int first = 0;
	static char c[20];
	
//	c = (char*)malloc(20);
//	ft_bzero(c,20);

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

void ft_init_readline(t_readline *readline)
{
	// struct termios termios;
	// struct termios old;
	int load_term;

	readline->term_type = getenv("TERM");
	load_term = tgetent(NULL, readline->term_type);
	if (!load_term)
	{
		ft_putstr_fd("Could not access to the termcap database orterminal \
		type is not defined in termcap\n",
					 1);
		// return (NULL);
	}
	readline->path = ttyname(1);
	readline->term_fd = open(readline->path, O_RDWR /* | O_NOCTTY | O_NDELAY*/);
	if (!isatty(readline->term_fd))
	{
		ft_putstr_fd("file descriptors not point to terminal\n", 1);
		// return (NULL);
	}
	/// config terminal
	//readline.old_termios = malloc(sizeof(struct termios));


	// tcgetattr(readline.term_fd, &old); /// save termios first state
	// tcgetattr(readline.term_fd, &termios);
	// termios.c_lflag &= ~(ECHO | ICANON | ISIG);
	// tcsetattr(readline.term_fd, TCSANOW, &termios);


	readline->colums_count = tgetnum("co");
	readline->line_count = tgetnum("li");
	readline->cursor.col_position = 0;
	readline->cursor.line_postion = 0;
	readline->line = NULL;	
	// return (readline);
}

char *create_line_from_chars_list(t_char_list *char_list)
{
	char *line;
	int i;
	int len;
	t_char_list *tmp;

	i = 0;
	len = char_list->len;
	tmp = char_list;
	if (!(line = (char*)malloc(len + 1)))
		ft_putstr_fd("line allocation problem", 1);
	ft_bzero(line, len + 1);
	while (tmp)
	{
		line[i] = tmp->value;
		tmp = tmp->next;
		i++;
	}
	line[len] = '\0';
	return line;
}

int get_char_list_lenght(t_char_list *char_list)
{
	int i;

	i = 0;
	if (char_list == NULL)
		return i;
	else
	{
		while (char_list)
		{
			i++;
			char_list = char_list->next;
		}
		return i;
	}
}

t_char_list *ft_copy_char_list(t_char_list *char_list)
{
	int len;
	t_char_list *tmp;
	t_char_list *copy;
	t_char_list *origin;

	origin = char_list;
	copy = NULL;
	len = get_char_list_lenght(origin);
	if (len == 0)
		return NULL;
	else
	{
		copy = (t_char_list*)malloc(sizeof(t_char_list));
		tmp = copy;
		while (len > 0)
		{
			tmp->len = origin->len;
			tmp->value = origin->value;
			tmp->next = NULL;
			origin = origin->next;
			len--;
			if (len > 0)
			{
				tmp->next = (t_char_list*)malloc(sizeof(t_char_list));
				tmp = tmp->next;
			}
		}
		return copy;
	}
}
t_lines_list *ft_insert_node_to_history(t_lines_list *list, t_lines_list *current, int history) ///
{
	t_lines_list *tmp;
	t_lines_list *tmp_list;

	tmp = NULL;
	tmp_list = NULL;
	if (!list)
	{
		current->history = history;
		//if (current->char_list)
		//{
		//	current->up_or_down = true;
		//	current->origin_char_list = ft_copy_char_list(current->char_list);
		//}
		current->index = 1;
		list = current;
		list->next = NULL;
		list->prev = NULL;
		return list;
	}
	else
	{
		tmp_list = list;
		if (current)
		{
			
			if (list->prev != NULL)
			{
				while (list->prev != NULL)
				{
					list = list->prev;
				}
			}
			//if (current->char_list)
			//{
			//	current->up_or_down = true;
			//	current->origin_char_list = ft_copy_char_list(current->char_list);
			//}
			current->history = history;
			current->next = list;
			current->index = list->index + 1;
			current->prev = NULL;
			list->prev = current;
			list = current;
		}
	}
	return list;
}


void ft_move_cursor_and_clear(t_cursor cursor)
{
	char *move_cursor;
	move_cursor = tgetstr("cm", NULL);
	ft_putstr_fd(tgoto(move_cursor, cursor.col_position - 1,
					   cursor.line_postion - 1),
				 1);
	ft_putstr_fd(tgetstr("cd", NULL), 1);
}
t_char_list *ft_free_char_list(t_char_list *char_list)
{
	t_char_list *tmp;
	
	tmp = NULL;
	while (char_list)
	{
		tmp = char_list;
		char_list = char_list->next;
		free(tmp);
		tmp = NULL;
	}
	return NULL;
}

void ft_add_to_char_list(t_readline *readline, char c, t_char_list **chars_list)
{
	t_char_list *tmp;

	tmp = NULL;
	ft_move_cursor_and_clear(readline->cursor);
	if (*chars_list == NULL || ( (*chars_list) && (*chars_list)->value == 0))
	{
		ft_putchar_fd(c, 1);
		if ((*chars_list))
			free(*chars_list);
		*chars_list = (t_char_list*)malloc(sizeof(t_char_list));
		(*chars_list)->value = c;
		(*chars_list)->len = 1;
		(*chars_list)->next = NULL;
	}
	else
	{
		tmp = *chars_list;
		while (tmp->next)
		{
			ft_putchar_fd(tmp->value, 1);
			tmp = tmp->next;
		}
		ft_putchar_fd(tmp->value, 1);
		tmp->next = (t_char_list*)malloc(sizeof(t_char_list));
		tmp->next->next = NULL;
		tmp->next->value = c;
		tmp->next->len = tmp->len + 1;
		(*chars_list)->len = tmp->next->len;
		ft_putchar_fd(c, 1);
	}
}

t_lines_list *ft_create_line_node(void)
{
	t_lines_list *ret;

	ret = (t_lines_list*)malloc(sizeof(t_lines_list));
	//ft_bzero(ret,sizeof(t_lines_list));
	ret->char_list = NULL;
	ret->origin_char_list = NULL;
	ret->up_or_down = false;
	ret->next = NULL;
	ret->prev = NULL;
	ret->value = 0;
	ret->history = 0;
	return ret;
}

t_lines_list *ft_delete_node_from_list(t_lines_list *current)
{
	t_char_list *tmp;
	t_char_list *tmp1;
	t_char_list *tmp_char_list;
	t_lines_list *next_line;
	
	next_line = NULL;
	if (current == NULL)
	{
		return NULL;
	}
	//tmp = current->char_list;
	//tmp_char_list = current->origin_char_list;

	if (current->next)
		current->next->prev = current->prev;
	//while (tmp)
	//{
	//	tmp1 = tmp;
	//	tmp = tmp->next;
	//	free(tmp1);
	//	tmp1 = NULL;
	//}
	//while (tmp_char_list)
	//{
	//	tmp1 = tmp_char_list;
	//	tmp_char_list = tmp_char_list->next;
	//	free(tmp1);
	//	tmp1 = NULL;
	//}
	if (current->next)
		next_line = current->next;
	//if (current)
	//	free(current);
	//current = NULL;
	current = ft_destory_node(current);
	return next_line;
}
int add_curr_node_to_history(t_readline *readline, long c,
				  t_lines_list *current, t_lines_list **history)
{
	t_lines_list *new_line_node;

	new_line_node = NULL;
	new_line_node = ft_create_line_node();
	new_line_node->char_list = ft_copy_char_list(current->char_list);
	new_line_node->origin_char_list = ft_copy_char_list(current->char_list);
	ft_delete_char_list(current->char_list);
	current->char_list = ft_copy_char_list(current->origin_char_list);
	*history = ft_delete_node_from_list(*history);
	if (new_line_node && new_line_node->char_list && new_line_node->char_list->value != 0)
	{
		*history = ft_insert_node_to_history(*history, new_line_node, 0);
		readline->line = create_line_from_chars_list(new_line_node->char_list);
	}
	else
	{
		////ft_delete_node_from_list(new_line_node);
		ft_destory_node(new_line_node);
	}
	return 0;
}

void  get_charcter(t_readline *readline, long c,t_lines_list *current)
{
	ft_add_to_char_list(readline, c, &current->char_list);
}

t_char_list *init_character_list(void)
{
	t_char_list *tmp;

	tmp = (t_char_list*)malloc(sizeof(t_char_list));
	tmp->len = 0;
	tmp->next = NULL;
	tmp->value = 0;
	return tmp;
}

t_lines_list *ft_up_in_history(t_readline *readline, t_lines_list **history)
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
	return *history;
}

t_lines_list *ft_down_in_history(t_readline *readline, t_lines_list **history)
{
	
	ft_move_cursor_and_clear(readline->cursor);
	if ((*history))//->prev)
	{
		if ((*history)->prev != NULL)
		{
			*history = (*history)->prev;
			(*history)->up_or_down = true;
		}
		ft_print_char_list((*history)->char_list);
	}
	return *history;
}

void ft_delete_last_charactere_from_line(t_lines_list **current, t_readline *readline)
{
	int len;
	t_char_list *tmp;
	int i;

	i = 1;
	len = get_char_list_lenght((*current)->char_list);
	tmp = (*current)->char_list;
	if (len == 0)
		return;
	else
	{
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
	}
	ft_move_cursor_and_clear(readline->cursor);
	if ((*current)->char_list)
		ft_print_char_list((*current)->char_list);
}

void	reset_terminal(struct termios old_termios, int fd)
{
	tcsetattr(fd, TCSANOW, &old_termios);
	
}

void	set_terminal(struct termios *termios, struct termios *old, int fd)
{
	tcgetattr(fd, old); /// save termios first state
	tcgetattr(fd, termios);
	termios->c_lflag &= ~(ECHO | ICANON | ISIG);
	tcsetattr(fd, TCSANOW, termios);
}

char	*ft_get_input(t_readline readline,int *status,struct termios old_term)
{
	long			character;
	t_lines_list	*current;
	int				newline_break;
	char			*line;
	static bool		inisialize_history = false;

	current = ft_create_line_node();
	character = 0;
	line = NULL;
	if (inisialize_history == false)
	{
		g_vars.history = NULL;
		inisialize_history = true;
	}
	current->next = g_vars.history;
	if (g_vars.history)
	{
		g_vars.history->prev = current;
	}
	g_vars.history = current;
	newline_break = 1;
	while (newline_break)
	{
		read(0, &character, 6);
		if (current && character == D_KEY_UP)
			current = ft_up_in_history(&readline, &current);
		else if (current &&character == D_KEY_DOWN)
			current = ft_down_in_history(&readline, &current);
		else if (character == D_KEY_ENTER)
			newline_break = add_curr_node_to_history(&readline, character, current, &g_vars.history);
		else if (character == D_KEY_BACKSPACE)
			ft_delete_last_charactere_from_line(&current, &readline);
		else if (character == D_KEY_CTRL_C)
		{
			/*
			if (current && current->history == 1 && (current->char_list != NULL && current->char_list->value != 0))
			{
				ft_delete_char_list(current->char_list);
				current->char_list = NULL;
			}*/
			if(current && current->char_list == NULL && current->origin_char_list == NULL && current->prev == NULL)
				g_vars.history = ft_delete_node_from_list(current);
			else
			{
				ft_delete_char_list(current->char_list);
				current->char_list = ft_copy_char_list(current->origin_char_list);
				g_vars.history = ft_delete_node_from_list(g_vars.history);
			}
			newline_break = 0;
			*status = 1;
		}
		else if (character == D_KEY_CTRL_D)
		{
			if (current && (current->char_list == NULL || current->char_list->value == 0))
			{
				if (g_vars.history)
					g_vars.history = ft_destroy_history(g_vars.history);
				if (g_vars.cmd)
					ft_destroy_ast(g_vars.cmd);
				reset_terminal(old_term, readline.term_fd);
				*status = 127;
				ft_putstr_fd("exit",1);
				exit(*status);
			}
		}
		else if (ft_isprint(character))
			get_charcter(&readline, character, current);
		if (newline_break == 0)
			ft_putstr_fd("\n", 1);
		character = 0;
	}
	//ft_print_lines_list(g_vars.history);
	character = 0;
	if (readline.line)
	{
		line = ft_strdup(readline.line);
		free(readline.line);
		readline.line = NULL;
	}
	return line;
}


int micro_read_line(char **line,int *status)
{
	
	struct termios termios;
	struct termios old_termios;
	t_readline readline;
	
	
	ft_init_readline(&readline);
	set_terminal(&termios, &old_termios, readline.term_fd);

	ft_get_cursor_position(&readline.cursor.line_postion,
						   &readline.cursor.col_position);
	*line  = ft_get_input(readline,status,old_termios);
	
	reset_terminal(old_termios, readline.term_fd);
	close(readline.term_fd);
	return 1;
}
