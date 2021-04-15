/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/15 16:48:45 by zjamali          ###   ########.fr       */
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

t_lines_list   *ft_insert_line(t_lines_list *list,char *line) /// 
{
	t_lines_list *tmp;
	
	tmp = NULL;
	if (!list)
	{
		list = malloc(sizeof(t_lines_list));
		list->value = ft_strdup(line);
		list->next = NULL;
		list->index = 0;
		list->prev = NULL;
	}
	else
	{
		tmp = malloc(sizeof(t_lines_list));
		tmp->value = ft_strdup(line);
		tmp->next = list;
		tmp->index = list->index + 1;
		tmp->prev = NULL;
		list->prev = tmp;
		list = tmp;
	}
	return list;
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
		ft_print_char_list(*chars_list);
		tmp = *chars_list;
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = malloc(sizeof(t_char_list));
		tmp->next->next = NULL;
		tmp->next->value = c;
		tmp->next->len = tmp->len + 1;
		(*chars_list)->len = tmp->next->len;
		//ft_putchar_fd(tmp->value,1);
		*chars_list = tmp;
	}
}
void ft_clear_char_list(t_char_list *char_list)
{
	t_char_list *tmp;
	while (char_list)
	{
		tmp = char_list;
		char_list = char_list->next;
		free(tmp);
	}
}
char *create_line_from_chars_list(t_char_list **char_list)
{
	char *line;
	int i;
	int len;
	t_char_list *tmp;

	
	i  = 0;
	len = (*char_list)->len;
	tmp = *char_list;
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

int  get_charctere(t_readline *readline, long c, 
							t_lines_list **lines_list,t_char_list **char_list)
{
	char *line;
	int newline_break;
	line = NULL;
	newline_break = 1;

	if (ft_isprint(c))
	{
		ft_add_to_char_list(readline, c,char_list);
	}
	else if (c == D_KEY_ENTER)
	{
		if ((*lines_list)->char_list)
			line = create_line_from_chars_list(char_list);
		*lines_list =  ft_insert_line(*lines_list,line);
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

void ft_up_in_lines(t_lines_list  **current)
{
	if (!*current)
		return;
	else
	{
		if (!(*current)->prev)
			ft_putstr_fd((*current)->value,1);
		
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

int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	t_lines_list *current;
	t_char_list *chars_list;
	long character;
	int newline_break;

	character = 0;
	lines_list = NULL;
	readline  = ft_init_readline(&termios);
	
	lines_list = ft_init_line_list();
	current = lines_list;
	chars_list = NULL;
	while(1)
	{
		//current = ft_create_node(&current);
		//create node
		//insertattail
		//node->prev = null node-<next=NULL 
		//**tmp tm = tmp->prev
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
				ft_up_in_lines(&current);
			else
				newline_break = get_charctere(readline,character,&current,&(current->char_list));
			character = 0;
			if (newline_break == 0)
				ft_putstr_fd("\n",1);
		}
		//ft_print_lines_list(lines_list);
	}
}