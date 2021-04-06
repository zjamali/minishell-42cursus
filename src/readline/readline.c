/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/06 19:55:40 by zjamali          ###   ########.fr       */
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

char *get_last_line(t_lines_list *lines_list)
{
	if (lines_list)
	{
		ft_putstr_fd("zbi",1);
		return lines_list->value;
	}
	return NULL;
}

char *get_line(t_cursor cursor,t_lines_list *lines_list)
{
	char *str;
	char *cc;
	char *tmp;

	tmp = NULL;
	long c;
	c  = 0;
	str = NULL;
	cc = malloc(2);
	cc[1] = '\0';
	char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
	while (1)
	{
		read(0,&c,6);
		if (c == 4283163)
		{
			ft_putstr_fd(tgoto ( cm_cap , cursor.col_position ,  cursor.line_postion),1);
			ft_putstr_fd(tgetstr("cd", NULL),1);
			//str = ft_strdup("UP");
			str = get_last_line(lines_list);
		}
		else if (ft_isprint(c) || c == '\n')
		{
			tmp = str;
			cc[0] = c;
			if (c != '\n')
				str = ft_strjoin(str,cc);
			if (c == '\n')
				break;
		}
		else if (c == 127) 		// delete = 127
		{
			ft_putstr_fd(tgoto ( cm_cap , cursor.col_position ,  cursor.line_postion ),1);// 
			int len = ft_strlen(str);
			ft_putnbr_fd(len,1);
			if (len > 0)
				str[len - 1] = '\0';
		}
		ft_putstr_fd(tgoto ( cm_cap , cursor.col_position -1 ,  cursor.line_postion -1 ),1);
		ft_putstr_fd(tgetstr("cd", NULL),1);
		if (str)
			ft_putstr_fd(str,1);
		//tputs()
	//	printf("%s",str);
	//	ft_putstr_fd("\n",1);
	//	//printf("%ld\n", c);
		c = 0;
	}
	//ft_putstr_fd("\nline-> ",1);
	//ft_putstr_fd(str,1);
	//ft_putstr_fd("\n",1);
	//termios.c_lflag 
	return (str);
}

t_lines_list   *insert_lines(t_lines_list *list,char *line)
{
	t_lines_list *tmp;
	
	if (!list)
	{
		tmp = malloc(sizeof(t_lines_list));
		tmp->value = line;
		tmp->next = 0;
	}
	else
	{
		tmp = malloc(sizeof(t_lines_list));
		tmp->value = line;
		tmp->next = list;
	}
	return tmp;
}

int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	//char *charactere;
	char *line;
	line = NULL;
	readline  = ft_init_readline(&termios);
	//printf("line : %d ; col : %d\n",readline->cursor.line_postion,readline->cursor.col_position);
	lines_list = NULL;
	while(1)
	{
		show_prompt();
		ft_get_cursor_position(&readline->cursor.line_postion,
		&readline->cursor.col_position);
		line = get_line(readline->cursor,lines_list);
		if ( line[1] != '\0')
			write(1, "\n",1);
		lines_list = insert_lines(lines_list,line);
	}
}