/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/10 18:22:13 by zjamali          ###   ########.fr       */
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
	ft_putstr_fd("minishell$> ",1);
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
		list->prev = NULL;
	}
	else
	{
		tmp = malloc(sizeof(t_lines_list));
		tmp->value = ft_strdup(line);
		tmp->next = list;
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
t_lines_list *ft_reset(t_lines_list **list)
{
	t_lines_list *tmp;

	tmp = NULL;
	if (*list == NULL)
		return NULL;
	else
	{
		tmp = *list;
		while (tmp)
		{
			if(tmp->history == 1)
			{
				if (tmp->next)
				{	
					*list = tmp->next;
					free(tmp);
					tmp = NULL;
				}
			}
			tmp = tmp->prev;
		}
	}
	return *list;
}

t_char_list *ft_add_to_char_list(t_readline *readline,char c,t_char_list *chars_list)
{
	t_char_list *tmp;
	char *move_cursor;

	move_cursor = tgetstr("cm",NULL);
	ft_putstr_fd(tgoto(move_cursor,readline->cursor.col_position - 1,
		readline->cursor.line_postion -1),1);
	ft_putstr_fd(tgetstr("cd",NULL),1);

	tmp = NULL;
	if (chars_list == NULL)
	{
		ft_putchar_fd(c,1);
		chars_list = malloc(sizeof(t_char_list));
		chars_list->value = c;
		chars_list->len = 1;
		chars_list->next = NULL;
	}
	else
	{
		tmp = chars_list;
		while (tmp->next)
		{
			ft_putchar_fd(tmp->value,1);
			tmp = tmp->next;
		}
		tmp->next = malloc(sizeof(t_char_list));
		tmp->next->next = NULL;
		tmp->value = c;
		tmp->len = chars_list->len;
		chars_list->len = tmp->len + 1;
		ft_putchar_fd(c,1);
	}
	return chars_list;
}

char *create_line(t_char_list *char_list)
{
	char *line;
	int i;
	int len;

	
	i  = 0;
	len = char_list->len;
	
	line = malloc(sizeof(len + 1));
	line[len] = '\0';
	
	while (i < len)
	{
		line[i] = char_list->value;
		char_list = char_list->next; 
		i++;
	}
	return line;
}

void get_charctere(t_readline *readline,long c,t_char_list **chars_list,t_lines_list **lines_list)
{
	char *line;

	line = NULL;
	if (ft_isprint(c))
	{
		*chars_list = ft_add_to_char_list(readline,c,*chars_list);
	}
	if (c == D_KEY_ENTER)
	{
		//ft_putchar_fd((*chars_list)->value,1);
		line = create_line(*chars_list);
		//ft_putstr_fd(line,1);
		*lines_list =  ft_insert_line(*lines_list,line);
		ft_putstr_fd("\n",1);
		ft_print_lines_list(*lines_list);
	}
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

int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	t_lines_list *current;
	t_char_list *chars_list;
	long character;
	
	character = 0;
	lines_list = NULL;
	readline  = ft_init_readline(&termios);
	//char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
	current = lines_list;
	while(1)
	{
		show_prompt();
		ft_get_cursor_position(&readline->cursor.line_postion,
		&readline->cursor.col_position);
		chars_list = init_character_list();
		while (1)
		{
			read(0,&character,6);
			get_charctere(readline,character,&chars_list,&lines_list);
			character = 0;
		}
	}
}