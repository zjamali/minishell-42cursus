/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/05 18:31:33 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <termcap.h>

int init_terminal(void)
{
	char *term;
	int ret;
	
	term = getenv("TERM");
	ret = tgetent(NULL,term);

	return ret;
}
void ft_get_cursor_position(int *x,int *y)
{
	char *c;
	c = malloc(20);
	write(1,"\e[6n",10);
	read(1,&c,10);
	//printf("\e[6n");
	// fflush(stdout);
	//ft_putstr_fd(c,1);
}

int main(int ac,char **av,char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char *str;
	char *tmp;
	char *cc;
	int cursor[2];

	cc= malloc(2);
	cc[1] = '\0';
	str = NULL;
	tmp = NULL;
	struct termios termios;
	long c;
	c = 0;
	//char *str = getenv("TERM"); // get 
	//printf("%s\n",str);
	char *path = ttyname(1);
	//printf("%s\n",path);
	//char terminal[] = ;	
	int fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	//printf("%d\n",fd);
	//ft_putstr_fd("minshell$>",1);
	init_terminal();
	
	if(isatty(fd))
	{
		//printf("fd point to teminl\n");
		ft_get_cursor_position(&cursor[0],&cursor[1]);
		tcgetattr(fd,&termios);
		//printf("termios before -> %ld\n",termios.c_lflag);
		//termios.c_lflag = 536872135;
		termios.c_lflag &= ~(ECHO  | ICANON);
		//termios.c_lflag = termios.c_lflag - ECHO;
		tcsetattr(fd,TCSANOW,&termios);
		
		//char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
		/// delete = 127
		//while (1)
		//{
		//	read(0,&c,6);
		//	if (ft_isprint(c) || c == '\n')
		//	{
		//		tmp = str;
		//		cc[0] = c;
		//		str = ft_strjoin(str,cc);
		//		//write(1,&c,6);
		//		if (c == '\n')
		//			break;
		//	}
		//	if (c == 127)
		//	{
		//		ft_putstr_fd(tgoto ( cm_cap , 10 ,  1 ),1);// 
		//		int len = ft_strlen(str);
		//		if (len > 0)
		//			str[len - 1] = '\0';
		//	}
		//	ft_putstr_fd("",1);
		//	printf("%s",str);
		//	ft_putstr_fd("\n",1);
		//	//printf("%ld\n", c);
		//	c = 0;
		//}
		//ft_putstr_fd("line->",1);
		//ft_putstr_fd(str,1);
		//ft_putstr_fd("\n",1);
		//termios.c_lflag 
	}	
}