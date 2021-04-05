#include <stdio.h>
#include <unistd.h>
#include <termios.h>
# include <fcntl.h>

int main(int ac,char **av,char **env)
{
	(void)ac;
	(void)av;
	(void)env;
	char *str;
	char *tmp;
	char *cc;

	cc= malloc(2);
	cc[1] = '\0';
	str = NULL;
	tmp = NULL;
	struct termios termios;
	long c;
	c = 0;
	//char *str = getenv("TERM"); // get 
	printf("%s\n",str);
	char *path = ttyname(1);
	printf("%s\n",path);
	//char terminal[] = ;	
	int fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	printf("%d\n",fd);
	if(isatty(fd))
	{
		printf("fd point to teminl\n");
		tcgetattr(fd,&termios);
		printf("termios before -> %ld\n",termios.c_lflag);
		//termios.c_lflag = 536872135;
		termios.c_lflag &= ~(ECHO  | ICANON);
		//termios.c_lflag = termios.c_lflag - ECHO;
		tcsetattr(fd,TCSANOW,&termios);
		printf("minishell$");
		printf("after -> %ld\n",termios.c_lflag);
		
		char  * cm_cap  =  tgetstr ( "cm" ,  NULL );
		/// delete = 127
		while (1)
		{
			read(0,&c,6);
			if (ft_isprint(c) || c == '\n')
			{
				tmp = str;
				cc[0] = c;
				str = ft_strjoin(str,cc);
				//write(1,&c,6);
				if (c == '\n')
					break;
			}
			if (c == 127)
			{
				int len = ft_strlen(str);
				if (len > 0)
					str[len - 1] = '\0';
				//tputs ( tgoto ( cm_cap , 0 ,  1 ),  1 ,  putchar ); // 
			}
			ft_putstr_fd(str,1);
			ft_putstr_fd("\n",1);
			//printf("%ld\n", c);
			c = 0;
		}
		ft_putstr_fd("line->",1);
		ft_putstr_fd(str,1);
		ft_putstr_fd("\n",1);
		//termios.c_lflag 
	}	
}