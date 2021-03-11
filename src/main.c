/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/03/11 16:08:59 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include <stdio.h>
#include <string.h>

void read_command_list(char *line)
{
	get_next_line(&line);
}

void show_prompt()
{
	write(1,"minishell>",12);
}

int main(int ac,char **av, char **env)
{
	t_token_list *token_list;
	char *line;
	while (1)
	{
		show_prompt();
		read_command_list(&line);
	}
}
