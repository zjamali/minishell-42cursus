/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/04/24 15:11:20 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../headers/minishell.h"
#include "../headers/execution.h"

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
	t_pipe_line *current_pipe_line;
	t_env *head;
	char *line;
	int status;

	t_lines_list *lines_list;
	struct termios termios;
	t_readline *readline;
	lines_list = NULL;

	status = 0;
	tokens_list = NULL;
	line = NULL;
	(void)ac;
	(void)av;
	int i = 0;
	
	readline = ft_init_readline(&termios);
	cmd = NULL;
	init_env(&head, env);
	while (i == 0)
	{
		//i++;
		show_prompt();
		micro_read_line(&line, readline, lines_list);
		if ( line[1] != '\0')
			write(1, "\n",1);
		tokens_list = ft_lexer(line);
		free(line);
		cmd = ft_parser(tokens_list);
		if (cmd)
			current_pipe_line = cmd->childs;
		while (current_pipe_line)
		{
			ft_expanding(current_pipe_line,&head,status);
			ft_print_pipeline_cmd(current_pipe_line);
			//status = ft_execute(current_pipe_line, &head);
			current_pipe_line = current_pipe_line->next;
		}
		if (cmd)
			ft_destroy_ast(cmd);
		cmd = NULL;
		line = NULL;
	}
}
//ls > s > s > d > f > g > h >> j>> j>>  jj < j < j < j
// \e\c\h\o
// export s= 2116 sdf sdfdsf !=  sdf sdfdsf
// "\dkjjkd\"dghj" 


// export a=a b= c

// if new fih = edit
// else no edit