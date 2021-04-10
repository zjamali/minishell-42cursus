/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/10 13:20:44 by zjamali          ###   ########.fr       */
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

t_lines_list   *insert_lines(t_lines_list *list,char *line,int history) /// 
{
	t_lines_list *tmp;
	
	tmp = NULL;
	if (!list)
	{
		list = malloc(sizeof(t_lines_list));
		list->value = ft_strdup(line);
		list->next = NULL;
		list->prev = NULL;
		list->history = 0;
		if (history)
			list->history = 1;
	}
	else
	{
		tmp = malloc(sizeof(t_lines_list));
		tmp->value = ft_strdup(line);
		tmp->next = list;
		tmp->prev = NULL;
		list->prev = tmp;
		tmp->history = 0;
		if (history)
			tmp->history = 1;
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


int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	t_lines_list *current;
	char *line;
	int a;
	a = 0;
	char *cc;
	char *tmp;
	tmp = NULL;
	char *history;

	history = NULL;
	long c;
	c  = 0;
	cc = malloc(2);
	cc[1] = '\0';
	lines_list = NULL;
	line = NULL;
	readline  = ft_init_readline(&termios);
	char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
	//line = init_line();
	//current_line = lines_list;
	current = lines_list;
	while(1)
	{
		show_prompt();
		ft_get_cursor_position(&readline->cursor.line_postion,
		&readline->cursor.col_position);
		while (1)
		{
			read(0,&c,6);
			if (c == 4283163) /// UP 
			{
				if (a == 0) //// add history to list
				{
					if (history)
					{
						if (current->history == 1)
							current->value = history;
						else
							current = insert_lines(lines_list,history,1);
					}
					a = 1;
				}
				ft_putstr_fd(tgetstr("cd", NULL),1);
				if (current)
				{
					if  (a == 0 && current->prev == NULL)
					{
						//line = ft_strdup(current->value);
						line = current->value;
					}
					else
					{
						if (current->next)	
						{	
							current = current->next;
							//line = ft_strdup(current->value);
							line = current->value;
						}
					}
				}
				a = 1;
				c = 0;
			}
			else if (c == 4348699) /// DOWN 
			{
				ft_putstr_fd(tgetstr("cd", NULL),1);
				if (current)
				{
					if  (a == 0 && current->next == NULL)
						line = ft_strdup(current->value);
					else
					{
						if (current->prev)
						{
							current = current->prev;
							line = ft_strdup(current->value);
						}
					}
					a = 2;
				}
			}
			else if (ft_isprint(c) || c == '\n')
			{
				a = 0;
				tmp = line;
				cc[0] = c;
				if (cc[0] != '\n')
					line = ft_strjoin(line,cc);
				history = line;
				//free(tmp);
				cc[0] = 0;
				tmp = NULL;
				if (c == '\n')
				{
					c = 0;
					break;
				}
				//current = ft_reset(&current);
			}
			else if (c == 127) 		// delete = 127
			{
				ft_putstr_fd(tgoto ( cm_cap , readline->cursor.col_position,  readline->cursor.line_postion ),1);// 
				int len = ft_strlen(line);
				if (len > 0)
					line[len - 1] = '\0';
			}
			/// move cursor 
			ft_putstr_fd(tgoto ( cm_cap , readline->cursor.col_position - 1,  readline->cursor.line_postion - 1),1);
			/// clear the terminal
			ft_putstr_fd(tgetstr("cd", NULL),1);
			if (line)
				ft_putstr_fd(line,1);
			c = 0;
		}
		if (line)
		{
			lines_list = insert_lines(lines_list,line,0);
			//free(line);
			current = lines_list;
			line = NULL;
		}
		ft_putstr_fd("\n",1);
		ft_print_lines_list(lines_list);
	}
}