/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/26 20:03:23 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <stdio.h>
#include <string.h>

void read_command_list(char **line)
{
	get_next_line(&(*line));
}

void show_prompt()
{
	write(1,GREEN,ft_strlen(GREEN));
	write(1,"minishell$ ",strlen("minishell$ "));
	write(1,RESET,ft_strlen(RESET));
}

int main(int ac,char **av,char **env)
{
	t_token *tokens_list;
	t_command_list *cmd;
	char *line;

	//write(1,env[1],ft_strlen(env[1]));
	tokens_list = NULL;
	line = NULL;
	(void)ac;
	(void)av;
	(void)env;
	int i = 0;
	while (i == 0)
	{
		//i++;
		show_prompt();
		read_command_list(&line);
		if ( line[1] != '\0')
			write(1, "\n",1);
		tokens_list = ft_lexer(line);
		free(line);
		cmd = ft_parser(tokens_list);
		ft_expanding(&cmd->childs);
		ft_print_cmd_list(cmd);
		//ft_destroy_ast(cmd);
		cmd = NULL;
		line = NULL;
	}
}

//ls > s > s > d > f > g > h >> j>> j>>  jj < j < j < j
// \e\c\h\o
// export s= 2116 sdf sdfdsf !=  sdf sdfdsf
// 