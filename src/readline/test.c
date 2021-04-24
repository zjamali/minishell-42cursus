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

	cc = malloc(2);
	cc[1] = '\0';
	str = NULL;
	tmp = NULL;
	struct termios termios;
	long c;
	c = 0;

	char *path = ttyname(1);
	int fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	init_terminal();
	
	if(isatty(fd))
	{
		tcgetattr(fd,&termios);
		termios.c_lflag &= ~(ECHO  | ICANON);
		tcsetattr(fd,TCSANOW,&termios);
		
		ft_putstr_fd("minshell$>",1);
		ft_get_cursor_position(&cursor[0],&cursor[1]);
		char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
		while (1)
		{
			read(0,&c,6);
			write(0,&c,6);
			if (c == 4283163)
			{
				ft_putstr_fd(tgoto ( cm_cap , cursor[1] ,  cursor[0] ),1);
				ft_putstr_fd(tgetstr("cd", NULL),1);
				str = "UP";
				/// str = get_last_line(readline);
			}
			else if (ft_isprint(c) || c == '\n')
			{
				tmp = str;
				cc[0] = c;
				str = ft_strjoin(str,cc);
				if (c == '\n')
					break;
			}
			else if (c == 127) 		// delete = 127
			{
				ft_putstr_fd(tgoto ( cm_cap , cursor[1] ,  cursor[0] ),1);// 
				int len = ft_strlen(str);
				ft_putnbr_fd(len,1);
				if (len > 0)
					str[len - 1] = '\0';
			}
			ft_putstr_fd(tgoto ( cm_cap , cursor[1] -1 ,  cursor[0] -1 ),1);
			ft_putstr_fd(tgetstr("cd", NULL),1);
			if (str)
				ft_putstr_fd(str,1);
			//tputs()
		//	printf("%s",str);
		//	ft_putstr_fd("\n",1);
		//	//printf("%ld\n", c);
			c = 0;
		}
		ft_putstr_fd("\nline-> ",1);
		ft_putstr_fd(str,1);
		//ft_putstr_fd("\n",1);
		//termios.c_lflag 
	}	
}