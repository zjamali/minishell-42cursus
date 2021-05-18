/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/17 18:57:12 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../headers/minishell.h"
#include "../headers/execution.h"

char *get_last_argument_or_command(t_pipe_line *current_pipe_line){

	t_args *args;
	char **split;

	split = NULL;
	args = NULL;
	if (current_pipe_line->child->next != NULL)
	{
		return (NULL);
	}
	else {
		args = current_pipe_line->child->args;
		if (args == NULL)
		{
			ft_putstr_fd(current_pipe_line->child->command,1);
			return (ft_strdup(current_pipe_line->child->command));
		}
		else{
			while (args->next != NULL)
			{
				args = args->next;
			}
			if (!ft_strcmp(current_pipe_line->child->command,"export"))
			{
				split = ft_split(args->value,'=');
				free(split[1]);
				return (split[0]);
			}
			else {

				if (args->value)
					return ft_strdup(args->value);
				else
					return (ft_strdup(current_pipe_line->child->command));
			}
		}
	}
	return NULL;
}

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

t_readline *ft_destroy_read_line(t_readline *read_line)
{
	read_line->term_type = NULL;
	read_line->path = NULL;
	read_line->old_termios = NULL;
	free(read_line->line);
	read_line->line = NULL;
	free(read_line);
	read_line = NULL;

	return read_line;
}

t_lines_list *ft_destory_line(t_lines_list *node)
{
	if (node->char_list)
		ft_delete_char_list(node->char_list);
	node->char_list = NULL;
	if (node->origin_char_list)
		ft_delete_char_list(node->origin_char_list);
	node->origin_char_list = NULL;
	node->next = NULL;
	node->prev = NULL;
	free(node);
	node = NULL;
	return node;
}

t_lines_list *ft_destroy_line_list(t_lines_list *lines_list)
{
	t_lines_list *last_node;
	t_lines_list *tmp;
	

	tmp = NULL;
	last_node = lines_list;
	if (last_node->next != NULL)
	{
		while (last_node->next != NULL)
		{
			last_node = last_node->next;
		}
	}
	tmp = last_node;
	while (tmp)
	{
		last_node = tmp;
		//if (tmp)
			tmp = tmp->prev;
		//if (last_node)
		last_node = ft_destory_line(last_node);
	}
	return NULL;
}


int main(int ac,char **av,char **env)
{
	t_token *tokens_list;
	t_command_list *cmd;
	t_pipe_line *current_pipe_line;
	t_env *head;
	char *line;
	int status;
	char *last_argumet_or_command;
//
	current_pipe_line = NULL;
	t_lines_list *lines_list;
	 struct termios termios;
	 t_readline *readline;
	 last_argumet_or_command = NULL;

	//// get terminal window size
	
	//struct winsize window;
	
	char **last_env;
	last_env = (char **)malloc(sizeof(char) * 2);
	last_env[0] = NULL;
	last_env[1] = NULL;

	lines_list = NULL;
	status = 0;
	tokens_list = NULL;
	line = NULL;
	(void)ac;
	(void)av;
	(void)env;
	int i = 0;
	//
	head = NULL;
	cmd = NULL;
	readline = ft_init_readline(&termios);
	init_env(&head, env);   // 24 bytes allocated
	while (i == 0)
	{
		//i++;
		show_prompt();
		micro_read_line(&line, readline, &lines_list,&status);
		//read_command_list(&line);
		if (line)
		{
			tokens_list = ft_lexer(line);
			free(line);
			line = NULL;
		}
		if (tokens_list)
		{
			cmd = ft_parser(tokens_list,&status);
			tokens_list = NULL;
		}
		if (cmd)
			current_pipe_line = cmd->childs;
		while (current_pipe_line)
		{
			last_env[0] = ft_int_to_string(status); 
			ft_expanding(current_pipe_line,&head,last_env);
			ft_print_pipeline_cmd(current_pipe_line);
			ft_putstr_fd("-----------------------\n",1);
			//last_env[1] = get_last_argument_or_command(current_pipe_line);
			status = ft_execute(current_pipe_line, &head);
			ft_putstr_fd("******\n",1);
			current_pipe_line = current_pipe_line->next;
		}
		if (cmd)
		{
			ft_destroy_ast(cmd);
			ft_putstr_fd(RESET,ft_strlen(RESET));
		}
		cmd = NULL;
		//readline= ft_destroy_read_line(readline);
		//if (lines_list)
		//	lines_list = ft_destroy_line_list(lines_list);
	}
	return 0;
}
/*
> f1 bouvle
echo hello >''>f1 boucle 
echo helo > $jefh boucle infinie ambigious redirection
*/