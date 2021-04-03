/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 16:46:56 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/03 18:16:18 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include <termios.h>

int main(int ac,char **av,char **env)
{
	struct termios termios;
	
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
		printf("%ld",termios.c_lflag);
		while (1)
		{
			/* code */
		}
		
		//termios.c_lflag 
	}	
}