/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/03 18:59:16 by zjamali          ###   ########.fr       */
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
				if (split[0] == NULL)
					split[0] = ft_strdup("=");
				else
				{
					if (split[0][0] != args->value[0])
					{
						free(split[0]);
						split[0] = ft_strdup(args->value);
					}
				}
				free(split[1]);
				free(split);
				if (split[0])
					return (split[0]);
				else
					return (NULL);
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

void show_prompt()
{
	write(1,GREEN,ft_strlen(GREEN));
	write(1,"minishell$ ",strlen("minishell$ "));
	write(1,RESET,ft_strlen(RESET));
}

t_simple_cmd	*ft_delete_emty_simple_cmds(t_pipe_line **pipe_line)
{
    t_simple_cmd	*head;
	t_simple_cmd	*prev;
	t_simple_cmd	*temp;

	prev = NULL;
	head = (*pipe_line)->child;
	temp = head;
    while (temp != NULL && (!temp->command && temp->inside_quotes == 0)
			&& temp->args == NULL && temp->redirections == NULL)
    {
        head = temp->next;
		ft_destroy_simple(temp);
        temp = head;
    }
    while (temp != NULL)
    {
        while (temp != NULL && (temp->inside_quotes != 0 
				|| ( temp->command && temp->inside_quotes == 0)))
        {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL)
            return (head);
		if (prev)
		{
        	prev->next = temp->next;
			ft_destroy_simple(temp);
			temp = prev->next;
		}
		else
			temp = temp->next;     
    }
	return (head);
}

int main(int ac,char **av,char **env)
{
	t_token *tokens_list;
	t_pipe_line *current_pipe_line;
	t_env *head;
	char *line;
	int status;
	char *last_argumet_or_command;
	static char *last_env[2];

	current_pipe_line = NULL;
	g_vars.cmd = NULL;
	last_argumet_or_command = NULL;
	last_env[0] = ft_strdup("0");
	status = 0;
	tokens_list = NULL;
	line = NULL;
	(void)ac;
	(void)av;
	(void)env;

	head = NULL;

	//cmd = NULL;
	signal(SIGINT, ft_signal_handler);
	signal(SIGQUIT, ft_signal_handler);
	ft_init_env(&head, env);   // 24 bytes allocated
	while (1337)
	{
		show_prompt();
		micro_read_line(&line,&status);
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
			// ft_print_pipeline_cmd(current_pipe_line);
			if (current_pipe_line->child)
			{
				if (last_env[1])
					free(last_env[1]);
				last_env[1] = get_last_argument_or_command(current_pipe_line);
				//status = ft_execute(current_pipe_line, &head);
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
