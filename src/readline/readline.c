/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/03 19:29:52 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include <termios.h>

int main(int ac,char **av,char **env)
{
	struct termios termios;
	long c;
	c = 0;
	char *str = getenv("TERM"); // get 
	printf("%s\n",str);
	char *path = ttyname(1);
	printf("%s\n",path);
	//char terminal[] = ;	
	int fd = open(path, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if(isatty(fd))
	{
		printf("fd point to teminla\n");
		tcgetattr(fd,&termios);
		printf("%ld\n",termios.c_lflag);
		termios.c_lflag = 536872135;//&= ~(ECHO | ICANON);
		tcsetattr(fd,TCSANOW,&termios);
		printf("%ld\n",termios.c_lflag);
		while (1)
		{
			printf("minishell>");
			read(0,&c,6);
		}
		
		//termios.c_lflag 
	}	
}