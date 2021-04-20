/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/20 10:25:19 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void ft_get_cursor_position(int *x,int *y)
{
	char *c;
	int i = 0;
	int first = 0;
	c = malloc(20);
	c[20] = '\0';
	write(0,"\033[6n",4);
	read(0,c,19);
	while (c[i])
	{
		if(first == 0 && ft_isdigit(c[i]))
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
			return;
		}
		i++;	
	}
}

t_readline *ft_init_readline(struct termios *termios)
{
	t_readline *readline;
	int load_term;
	if (!(readline = malloc(sizeof(t_readline))))
		return (NULL);
	readline->term_type = getenv("TERM");
	load_term = tgetent(NULL,readline->term_type);
	if (!load_term)
	{
		ft_putstr_fd("Could not access to the termcap database orterminal \
		type is not defined in termcap\n",1);
		return(NULL);
	}
	readline->path = ttyname(1);
	readline->term_fd = open(readline->path,O_RDWR/* | O_NOCTTY | O_NDELAY*/);
	if(!isatty(readline->term_fd))
	{
		ft_putstr_fd("file descriptors not point to terminal\n",1);
		return(NULL);
	}
	/// config terminal
	tcgetattr(readline->term_fd,termios);
	termios->c_lflag &= ~(ECHO | ICANON);
	tcsetattr(readline->term_fd,TCSANOW,termios);
	readline->colums_count = tgetnum("co");
	readline->line_count = tgetnum("li");
	readline->cursor.col_position = 0;
	readline->cursor.line_postion = 0;
	return (readline);
}

void show_prompt(void)
{
	write(1,GREEN,ft_strlen(GREEN));
	ft_putstr_fd("minishell$ ",1);
	write(1,RESET,ft_strlen(RESET));
}
char *create_line_from_chars_list(t_char_list *char_list)
{
	char *line;
	int i;
	int len;
	t_char_list *tmp;

	
	i  = 0;
	len = char_list->len;
	tmp = char_list;
	if (!(line = malloc(len + 1)))
		ft_putstr_fd("line allocation problem",1);
	ft_bzero(line,len + 1);
	while (tmp)
	{
		line[i] = tmp->value;
		tmp = tmp->next;
		i++;
	}
	line[len] = '\0';
	return line;
}

t_lines_list   *ft_insert_node_to_line_list(t_lines_list *list,t_lines_list *current,int history) /// 
{
	t_lines_list *tmp;
	
	tmp = NULL;
	if (!list)
	{
		current->prev = NULL;
		current->next = NULL;
		//if (current->char_list)
		//	current->value = create_line_from_chars_list(current->char_list);
		current->history = history;
		current->index = 1;
		list = current;
		return list;
	}
	else
	{
		if (current)
		{
			//if (current->char_list)
			//	current->value = create_line_from_chars_list(current->char_list);
			if (list->prev != NULL)
			{
				while (list->prev != NULL)
				{
					list = list->prev;
				}	
			}
			current->next = list;
			current->index = list->index + 1;
			current->prev = NULL;
			list->prev = current;
			list = current;
			return list;
		}
		else
		{
			return list;
		}
		
	}
}

void ft_print_lines_list(t_lines_list *lines_list)
{
	while (lines_list)
	{
		ft_putstr_fd("\nline-> ",1);
		ft_putstr_fd(lines_list->value,1);
		ft_putstr_fd("\n",1);
		lines_list = lines_list->next;
	}
}
void ft_move_cursor_and_clear(t_cursor cursor)
{
	char *move_cursor;
	
	move_cursor = tgetstr("cm",NULL);
	ft_putstr_fd(tgoto(move_cursor,cursor.col_position - 1,
		cursor.line_postion -1),1);
	ft_putstr_fd(tgetstr("cd",NULL),1);
	
}
void ft_print_char_list(t_char_list *chars_list)
{
	t_char_list *tmp;
	
	tmp = chars_list;

	while(tmp->next)
	{
		ft_putchar_fd(tmp->value,1);
		tmp = tmp->next;
	}
	if (tmp)
		ft_putchar_fd(tmp->value,1);
}

void ft_add_to_char_list(t_readline *readline,char c,t_char_list **chars_list)
{
	t_char_list *tmp;

	tmp = NULL;
	ft_move_cursor_and_clear(readline->cursor);
	if (*chars_list == NULL)
	{
		ft_putchar_fd(c,1);
		*chars_list = malloc(sizeof(t_char_list));
		(*chars_list)->value = c;
		(*chars_list)->len = 1;
		(*chars_list)->next = NULL;
	}
	else
	{
		tmp = *chars_list;
		while(tmp->next)
		{
			ft_putchar_fd(tmp->value,1);
			tmp = tmp->next;
		}
		ft_putchar_fd(tmp->value,1);
		tmp->next = malloc(sizeof(t_char_list));
		tmp->next->next = NULL;
		tmp->next->value = c;
		tmp->next->len = tmp->len + 1;
		(*chars_list)->len = tmp->next->len;
		ft_putchar_fd(c,1);
	}
}

t_lines_list *ft_create_node(void)
{	
	t_lines_list *ret;

		ret = malloc(sizeof(t_lines_list));
		ret->char_list = NULL;
		ret->origin_char_list = NULL;
		ret->up_or_down = false;
		ret->next = NULL;
		ret->prev = NULL;
		ret->value = NULL;
		return ret;
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
		copy = malloc(sizeof(t_char_list));
		tmp = copy;
		while (len > 0)
		{
			tmp->len = origin->len;
			tmp->value = origin->value;
			tmp->next = NULL;
			origin = origin->next;
			len--;
			if(len > 0)
			{
				tmp = tmp->next;
				tmp = malloc(sizeof(t_char_list));
			}
		}
		return copy;
	}
}

int  get_charctere(t_readline *readline, long c, 
							t_lines_list *current,t_lines_list **lines_list)
{
	char *line;
	int newline_break;
	line = NULL;
	newline_break = 1;
	
	if (ft_isprint(c))
	{
		
		if ((*lines_list) && (*lines_list)->up_or_down == true)
			ft_add_to_char_list(readline, c,&(*lines_list)->char_list);
		else
			ft_add_to_char_list(readline, c,&current->char_list);
	}
	else if (c == D_KEY_ENTER)
	{
		if ((*lines_list) && (*lines_list)->up_or_down == true)
		{
			current = ft_create_node();
			current->char_list = ft_copy_char_list((*lines_list)->char_list);
			(*lines_list)->char_list = ft_copy_char_list((*lines_list)->origin_char_list);
			ft_insert_node_to_line_list(*lines_list,current,0);
			current = NULL;
		}
		else
		{
			if (current->char_list)
				*lines_list =  ft_insert_node_to_line_list(*lines_list,current,0);
		}
		newline_break = 0;
	}
	return newline_break;
}

t_char_list *init_character_list(void)
{
	t_char_list *tmp;
	tmp = malloc(sizeof(t_char_list));
	tmp->len = 0;
	tmp->next = NULL;
	tmp->value = 0;
	return tmp;
}



void ft_up_in_lines(t_readline *readline,t_lines_list  **current)
{
	if (!*current)
	{
		return;
	}
	else
	{
		ft_move_cursor_and_clear(readline->cursor);
		if ((*current)->next)
		{
			(*current) = (*current)->next;
			(*current)->up_or_down = true;
			if ((*current)->char_list)
			{
				(*current)->origin_char_list = ft_copy_char_list((*current)->char_list);
				ft_print_char_list((*current)->char_list);
			}
			else /// skip emty char_list
			{
				if ((*current)->next)
				{
					(*current) = (*current)->next;
					(*current)->up_or_down = true;
					(*current)->origin_char_list = ft_copy_char_list((*current)->char_list);
					ft_print_char_list((*current)->char_list);
				}
			}
		}
		else
		{
			(*current)->up_or_down = true;
			if ((*current)->char_list)
			{
				(*current)->origin_char_list = ft_copy_char_list((*current)->char_list);
				ft_print_char_list((*current)->char_list);
			}
		}
	
	}
}
void ft_down_in_lines(t_readline *readline,t_lines_list  **current)
{
	if (!*current)
		return;
	else
	{
		ft_move_cursor_and_clear(readline->cursor);
		if ((*current)->prev)
		{
			(*current) = (*current)->prev;
			if ((*current)->char_list)
				ft_print_char_list((*current)->char_list);
			else
			{
				if ((*current)->prev)
				{
					(*current) = (*current)->prev;
					if ((*current)->char_list)
						ft_print_char_list((*current)->char_list);
				}
			}			
		}
		else
		{
			if ((*current)->char_list)
				ft_print_char_list((*current)->char_list);
		}
	
	}
	
}

t_lines_list *ft_init_line_list(void)
{
	t_lines_list *lines_list;

	lines_list = malloc(sizeof(t_lines_list));
	if (!lines_list)
	{
		ft_putstr_fd("line list allocation problem",1);
		return (NULL);
	}
	lines_list->char_list = NULL;
	lines_list->index = 0;
	lines_list->next = NULL;
	lines_list->prev = NULL;
	lines_list->value = NULL;
	lines_list->history = 0;
	return lines_list;
}



void ft_delete(t_lines_list **current,t_readline *readline)
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
		while (len - 1> i)
		{
			i++;
			tmp = tmp->next;
		}
		free(tmp->next);
		tmp->next = NULL;
	}
	ft_move_cursor_and_clear(readline->cursor);
	ft_print_char_list((*current)->char_list);
}

int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	t_lines_list *current;
	long character;
	int newline_break;

	character = 0;
	readline  = ft_init_readline(&termios);
	lines_list = NULL;
	while(1)
	{
		current = ft_create_node();
		int fd;
		newline_break = 1;
		show_prompt();
		ft_get_cursor_position(&readline->cursor.line_postion,
		&readline->cursor.col_position);
		while (newline_break)
		{
			fd = open("file_debuging", O_RDWR| O_APPEND| O_CREAT,0666);
			read(0,&character,6);
			dprintf(fd,"%ld\n",character);
			close(fd);
			if (character == D_KEY_UP)
			{
				if (current && current->next == NULL && current->next == NULL)
				{
					lines_list = ft_insert_node_to_line_list(lines_list,current,1);
					current = NULL;
				}
				ft_up_in_lines(readline,&lines_list);
			}
			else if (character == D_KEY_DOWN)
			{
				if (current && current->next == NULL && current->next == NULL)
				{
					lines_list = ft_insert_node_to_line_list(lines_list,current,1);
					current = NULL;
				}
				ft_down_in_lines(readline,&lines_list);
			}
			else if (character == D_KEY_BACKSPACE)
			{
				ft_delete(&current,readline);
			}
			else
			{
				if (current)
				{
					newline_break = get_charctere(readline,character,current,&lines_list);
				}
				else
				{
					fd = open("file_debuging", O_RDWR| O_APPEND| O_CREAT,0666);
					dprintf(fd,"%s\n","JADID");
					close(fd);
					if (!current)
						current = ft_create_node();
					
					///current = lines_list;
					//if (current->edit_char_list == NULL)
					//	current->edit_char_list = current->char_list;
					newline_break = get_charctere(readline,character,current,&lines_list);
				}
				
			}
			character = 0;
			if (newline_break == 0)
				ft_putstr_fd("\n",1);
			
		}
	}
}