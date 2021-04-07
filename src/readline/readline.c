/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/07 18:13:22 by zjamali          ###   ########.fr       */
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
		return lines_list->value;
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
	//cc[1] = '\0';
	char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
	while (1)
	{
		read(0,&c,6);
		if (c == 4283163) /// UP 
		{
			ft_putstr_fd(tgoto ( cm_cap , cursor.col_position ,  cursor.line_postion),1);
			ft_putstr_fd(tgetstr("cd", NULL),1);
			tmp = str;
			//str = get_last_line(*current_line);
			//if (!str)
			//	str= ft_strdup("why");
			//}
			c = 0;
		}
		//else if (c == 4348699) /// DOWN 
		//{
		//	ft_putstr_fd(tgoto ( cm_cap , cursor.col_position ,  cursor.line_postion),1);
		//	ft_putstr_fd(tgetstr("cd", NULL),1);
		//	//str = ft_strdup("UP");
		//	//str = get_last_line(lines_list);
		//	//tmp = str;
		//	if (currrent_line)
		//	{
		//		str = get_last_line(currrent_line,index);
		//		currrent_line = currrent_line->prev;
		//		index++;
		//	}
		//}
		else if (ft_isprint(c) || c == '\n')
		{
			tmp = str;
			cc[0] = c;
			cc[1] = '\0';
			if (cc[0] != '\n')
				str = ft_strjoin(str,cc);
			free(tmp);
			tmp = NULL;
			if (c == '\n')
			{
				c = 0;
				break;
			}
		}
		else if (c == 127) 		// delete = 127
		{
			ft_putstr_fd(tgoto ( cm_cap , cursor.col_position ,  cursor.line_postion ),1);// 
			int len = ft_strlen(str);
			ft_putnbr_fd(len,1);
			if (len > 0)
				str[len - 1] = '\0';
		}
		/// move cursor 
		ft_putstr_fd(tgoto ( cm_cap , cursor.col_position -1 ,  cursor.line_postion -1 ),1);
		/// clear the terminal
		ft_putstr_fd(tgetstr("cd", NULL),1);
		if (str)
		{
			//ft_putnbr_fd(index,1);
			ft_putstr_fd(str,1);
		}
		//tputs()
	//	printf("%s",str);
	//	ft_putstr_fd("\n",1);
	//	//printf("%ld\n", c);
		c = 0;
		//ft_putnbr_fd(index,1);
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
	
	tmp = NULL;
	if (!list)
	{
		list = malloc(sizeof(t_lines_list));
		list->value = ft_strdup(line);
		list->index = 1;
		list->next = NULL;
		list->prev = NULL;
	}
	else
	{
		tmp = list;
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = malloc(sizeof(t_lines_list));
		tmp->next->value = ft_strdup(line);
		tmp->next->next = NULL;
		tmp->next->index = tmp->index + 1;
		tmp->next->prev = tmp;
	}
	return list;
}
void ft_print_lines_list(t_lines_list *lines_list)
{
	while (lines_list)
	{
		ft_putstr_fd("\nline-> ",1);
		ft_putstr_fd(lines_list->value,1);
		ft_putstr_fd("\t\t index : ",1);
		ft_putnbr_fd(lines_list->index,1);
		ft_putstr_fd("\n",1);
		lines_list = lines_list->next;
	}
	return tmp;
}

int main()
{
	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
//	char *charactere;
	char *line;
	
	lines_list = NULL;
	line = 0;
	readline  = ft_init_readline(&termios);
	//printf("line : %d ; col : %d\n",readline->cursor.line_postion,readline->cursor.col_position);
	//line = init_line();
	//current_line = lines_list;
	while(1)
	{
		show_prompt();
		ft_get_cursor_position(&readline->cursor.line_postion,
		&readline->cursor.col_position);
		line = get_line(readline->cursor,lines_list);
		if (line)
			lines_list = insert_lines(lines_list,line);
		ft_putstr_fd("\n",1);
		ft_print_lines_list(lines_list);
		//free(line);
	}
}