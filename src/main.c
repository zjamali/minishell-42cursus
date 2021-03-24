/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/24 17:06:32 by mbari            ###   ########.fr       */
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

int main()
{
	t_token *tokens_list;
	t_command_list *cmd;
	char *line;

	tokens_list = NULL;
	line = NULL;
	while (1)
	{
		show_prompt();
		read_command_list(&line);
		if ( line[1] != '\0')
			write(1, "\n",1);
		tokens_list = ft_lexer(line);
		free(line);
		cmd = ft_parser(tokens_list);
	}
	free(line);
}