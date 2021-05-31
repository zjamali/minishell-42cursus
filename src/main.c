/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/31 13:30:42 by zjamali          ###   ########.fr       */
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
			if (current_pipe_line->child->command)
				return (ft_strdup(current_pipe_line->child->command));
		}
		else{
			while (args->next != NULL)
			{
				args = args->next;
			}
			if (current_pipe_line->child->command && !ft_strcmp(current_pipe_line->child->command,"export"))
			{
				split = ft_split(args->value,'=');
				free(split[1]);
				free(split);
				return (split[0]);
			}
			else {
				if (args->value)
					return ft_strdup(args->value);
				else
				{
					if (current_pipe_line->child->command)
						return (ft_strdup(current_pipe_line->child->command));
				}
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
/*
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
*/

t_simple_cmd *ft_delete_emty_simple_cmds(t_pipe_line **pipe_line)
{
	// Store head node
    t_simple_cmd *head;
	t_simple_cmd *prev;
	prev = NULL;

	head = (*pipe_line)->child;

	
	t_simple_cmd *temp;
	temp = head;
    while (temp != NULL && (!temp->command && temp->inside_quotes == 0) && temp->args == NULL && temp->redirections == NULL)
    {
        head = temp->next; // Changed head
        //free(temp); // free old head
		ft_destroy_simple(temp);
        temp = head; // Change Temp
    }
	
    // Delete occurrences other than head
    while (temp != NULL)
    {
        while (temp != NULL && (temp->inside_quotes != 0 || ( temp->command && temp->inside_quotes == 0)))
        {
            prev = temp;
            temp = temp->next;
        }
 
        // If key was not present in linked list
        if (temp == NULL)
            return head;
 
        // Unlink the node from linked list
		if (prev)
		{
        	prev->next = temp->next;
			ft_destroy_simple(temp);
			//free(temp); // Free memory
			temp = prev->next;
        	// Update Temp for next iteration of outer loop
		}
		else
		{
			temp = temp->next;
		}        
    }
	return head;
}

void ft_destroy_list(t_env *head)
{
	t_env *temp;

	temp = head;
	if (head == NULL)
		return ;
	while (temp != NULL)
	{
		free(temp->name);
		free(temp->value);
		free(temp);
		temp = temp->next;
	}
}

int main(int ac,char **av,char **env)
{
	t_token *tokens_list;
	//t_command_list *cmd;
	t_pipe_line *current_pipe_line;
	t_env *head;
	char *line;
	int status;
	char *last_argumet_or_command;
//
	current_pipe_line = NULL;
	g_vars.cmd = NULL;
	//struct termios termios;
	 last_argumet_or_command = NULL;

	//// get terminal window size
	
	//struct winsize window;
	
	//char **last_env;
	static char *last_env[2];
	
	//last_env = (char **)malloc(sizeof(char) * 3);
	//ft_bzero(last_env,sizeof(sizeof(char) * 3));
	last_env[0] = ft_strdup("0");
	//last_env[1] = NULL;
	//last_env[2] = NULL;

	status = 0;
	tokens_list = NULL;
	line = NULL;
	(void)ac;
	(void)av;
	(void)env;
	
	int i = 0;
	//
	head = NULL;
	//cmd = NULL;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	init_env(&head, env);   // 24 bytes allocated
	while (i == 0)
	{
		// i++;
		show_prompt();
		//micro_read_line(&line,&status);
		read_command_list(&line);
		
		if (line)
		{
			tokens_list = ft_lexer(line);
			free(line);
			line = NULL;
		}
		
		if (tokens_list)
		{
			g_vars.cmd = ft_parser(tokens_list,&status);
			tokens_list = NULL;
		}
		if (g_vars.cmd)
			current_pipe_line = g_vars.cmd->childs;
		while (current_pipe_line)
		{
			if (last_env[0])
				free(last_env[0]);
			last_env[0] = ft_int_to_string(status);
			ft_expanding(current_pipe_line,&head,last_env);
			current_pipe_line->child = ft_delete_emty_simple_cmds(&current_pipe_line);
			if (current_pipe_line->child)
			{
				//  ft_print_pipeline_cmd(current_pipe_line);
				if (last_env[1])
					free(last_env[1]);
				last_env[1] = get_last_argument_or_command(current_pipe_line);
				//ft_putchar_fd('\n', 1);
				status = ft_execute(current_pipe_line, &head);
			}
			current_pipe_line = current_pipe_line->next;
		}
		if (g_vars.cmd)
		{
			ft_destroy_ast(g_vars.cmd);
			ft_putstr_fd(RESET,ft_strlen(RESET));
			g_vars.cmd = NULL;
		}
	}
	ft_destroy_list(head);
	return 0;
}
/*
echo >      \;
> f1 bouvle
echo hello >''>f1 boucle 
echo helo > $jefh boucle infinie ambigious redirection
error :: cat ""
echo $\\$PWD
echo "$PWD_$PWD"
echo "$\\$PWD"
*/