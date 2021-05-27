/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/27 21:55:09 by zjamali          ###   ########.fr       */
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
		//	ft_putstr_fd(current_pipe_line->child->command,1);
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
				return (split[0]);
			}
			else {
				if (args->value)
					return ft_strdup(args->value);
				else
				{
					return (ft_strdup(current_pipe_line->child->command));
				}
			}
		}
	}
	return NULL;
}

void read_command_list(char **line)
{
//	get_next_line(&(*line));
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
t_lines_list *ft_destory_node(t_lines_list *node)
{
	if (node->char_list)
		ft_delete_char_list(node->char_list);
	node->char_list = NULL;
	if (node->origin_char_list)
		ft_delete_char_list(node->origin_char_list);
	node->origin_char_list = NULL;
	node->next = NULL;
	node->prev = NULL;
	if (node != NULL)
		free(node);  //////////////
	node = NULL;
	return NULL;
}

t_lines_list *ft_destroy_history(t_lines_list *history)
{
	t_lines_list *last_node;
	t_lines_list *tmp;
	

	tmp = NULL;
	last_node = history;
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
		last_node = ft_destory_node(last_node);
	}
	return NULL;
}
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
	
	//struct termios termios;
	 last_argumet_or_command = NULL;

	//// get terminal window size
	
	//struct winsize window;
	
	char **last_env;
	last_env = (char **)malloc(sizeof(char) * 2);
	last_env[0] = NULL;
	last_env[1] = NULL;

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
	
	init_env(&head, env);   // 24 bytes allocated
	while (i == 0)
	{
		//i++;
		show_prompt();
		micro_read_line(&line,&status);
		// read_command_list(&line);
		
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
			if (last_env[0])
			{
				free(last_env[0]);
				last_env[0] = NULL;
			}
			last_env[0] = ft_int_to_string(status); 
			ft_expanding(current_pipe_line,&head,last_env);
			current_pipe_line->child = ft_delete_emty_simple_cmds(&current_pipe_line);
			if (current_pipe_line->child)
			{
				// ft_print_pipeline_cmd(current_pipe_line);
				if (last_env[1])
				{
					free(last_env[1]);
					last_env = NULL;
					last_env[1] = get_last_argument_or_command(current_pipe_line);
				}
				//ft_putchar_fd('\n', 1);
				status = ft_execute(current_pipe_line, &head);
				if (last_env[0])
				{
					free(last_env[0]);
					last_env[0] = NULL;
				}
			}
			current_pipe_line = current_pipe_line->next;
		}
		if (cmd)
		{
			ft_destroy_ast(cmd);
			ft_putstr_fd(RESET,ft_strlen(RESET));
			cmd = NULL;
		}
		//readline= ft_destroy_read_line(readline);
		//readline= NULL;
		//if (history)
		//	history = ft_destroy_line_list(history);
	}
	return 0;
}
/*
echo >      \;
> f1 bouvle
echo hello >''>f1 boucle 
echo helo > $jefh boucle infinie ambigious redirection
*/