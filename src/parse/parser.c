/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 11:37:37 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/31 18:57:25 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

///#include "../headers/parser.h"
#include "../../headers/minishell.h"

void ft_destroy_redirection(t_redirection *redis)
{
	t_redirection *current_redi;
	t_redirection *parent_redi;

	parent_redi = redis;
	while (parent_redi)
	{
		current_redi = parent_redi;
		parent_redi = parent_redi->next;

		free(current_redi->file_name);
		free(current_redi);
	}
}

void	ft_destroy_simple(t_simple_cmd *cmd)
{
	t_args	*head;
	t_args	*current_arg;

	head = NULL;
	current_arg = NULL;
	if (cmd->command)
		free(cmd->command);
	head = cmd->args;
	while (head != NULL)
	{
		current_arg = head;
		head = head->next;
		free(current_arg->value);
		current_arg->value = NULL;
		free(current_arg);
	}
	current_arg = NULL;
	if (cmd->redirections)
		ft_destroy_redirection(cmd->redirections);
	cmd->redirections = NULL;
	free(cmd);
}

void	ft_destroy_pipe_line(t_pipe_line *pipe_line)
{
	t_simple_cmd	*current_cmd;
	t_simple_cmd	*parent_cmd;

	parent_cmd = pipe_line->child;
	while (parent_cmd)
	{
		current_cmd = parent_cmd;
		parent_cmd = parent_cmd->next;
		ft_destroy_simple(current_cmd);
		current_cmd = NULL;
	}
	free(pipe_line);
	pipe_line = NULL;
}

void	ft_destroy_ast(t_command_list *cmd_list)
{
	t_pipe_line	*current_pipe;
	t_pipe_line	*parent_pipe;

	parent_pipe = cmd_list->childs;
	while (parent_pipe)
	{
		current_pipe = parent_pipe;
		parent_pipe = parent_pipe->next;
		ft_destroy_pipe_line(current_pipe);
		current_pipe = NULL;
	}
	free(cmd_list);
}


/*****************************/

t_command_list	*init_cmd_list(void)
{
	t_command_list	*cmd_list;

	if (!(cmd_list = (t_command_list *)malloc(sizeof(t_command_list))))
		return (NULL);
	cmd_list->childs = NULL;
	cmd_list->pipe_line_count = 0;
	return (cmd_list);
}

t_redirection	*ft_create_redirection(t_token **tokens, int index)
{
	t_redirection *redirection;

	if (!(redirection = (t_redirection *)malloc(sizeof(t_redirection))))
		return NULL;
	redirection->index = index;
	redirection->next = NULL;
	if ((*tokens)->type == GREAT)
		redirection->type = RE_GREAT;
	else if ((*tokens)->type == DOUBLE_GREAT)
		redirection->type = RE_DOUBLE_GREAT;
	else if ((*tokens)->type == LESS)
		redirection->type = RE_LESS;
	(*tokens) = (*tokens)->next;
	redirection->file_name = ft_strdup((*tokens)->value);
	(*tokens) = (*tokens)->next;
	return (redirection);
}

t_redirection	*ft_insert_redirection(t_redirection *redirection,
				t_token **tokens, int index)
{
	t_redirection	*tmp;

	tmp = NULL;
	if (redirection == NULL)
		redirection = ft_create_redirection(tokens, index);
	else
	{
		tmp = redirection;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = ft_create_redirection(tokens, index);
	}
	return (redirection);
}

void	ft_insert_arg(t_args *head, t_args *current_args)
{
	t_args	*tmp;

	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_args;
}

t_args	*ft_create_arg(char *value)
{
	t_args	*arg;

	arg = (t_args *)malloc(sizeof(t_args));
	arg->value = ft_strdup(value);
	arg->next = NULL;
	arg->inside_quotes = 0;
	return (arg);
}

t_simple_cmd	*ft_create_simple_cmd(t_token **tokens)
{
	t_simple_cmd	*cmd;
	t_args			*tmp;
	int				i;
	int				r;
	int				j;

	j = 0;
	i = 0;
	r = 0;
	tmp = NULL;
	if (!(cmd = (t_simple_cmd *)malloc(sizeof(t_simple_cmd))))
		return NULL;
	cmd->args = NULL;
	cmd->command = NULL;
	cmd->next = NULL;
	cmd->args = NULL;
	cmd->redirections = NULL;
	while ((*tokens)->type != PIPE && (*tokens)->type != SEMI && (*tokens)->type != NEWLINE)
	{
		if ((*tokens)->type == GREAT || (*tokens)->type == DOUBLE_GREAT || (*tokens)->type == LESS)
		{
			cmd->redirections = ft_insert_redirection(cmd->redirections, tokens, r);
			r++;
		}
		else if ((*tokens)->type == WORD)
		{
			if (cmd->command == NULL)
			{
				cmd->command = strdup((*tokens)->value);
			}
			else
			{
				if (cmd->args == NULL)
				{
					cmd->args = ft_create_arg((*tokens)->value);
				}
				else
				{
					tmp = ft_create_arg((*tokens)->value);
					ft_insert_arg(cmd->args, tmp);
				}
			}
			(*tokens) = (*tokens)->next;
		}
		else
			(*tokens) = (*tokens)->next;
	}
	return (cmd);
}

void ft_insert_simple_cmd(t_simple_cmd *head, t_simple_cmd *current_cmd)
{
	t_simple_cmd *tmp;
	tmp = head;
	while (tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = current_cmd;
}

t_pipe_line *ft_create_pieline(t_token **tokens)
{
	t_pipe_line *pipe_line;
	t_simple_cmd *head;
	t_simple_cmd *current_cmd;

	if (!(pipe_line = (t_pipe_line *)malloc(sizeof(t_pipe_line))))
		return NULL;
	pipe_line->next = NULL;
	pipe_line->child = NULL;
	pipe_line->simple_cmd_count = 0;
	current_cmd = NULL;
	head = NULL;
	while ((*tokens)->type != NEWLINE)
	{
		if (head == NULL)
		{
			head = ft_create_simple_cmd(tokens);
			pipe_line->simple_cmd_count += 1;
		}
		else
		{
			if ((*tokens)->type == SEMI || (*tokens)->type == NEWLINE)
				break;
			(*tokens) = (*tokens)->next;
			current_cmd = ft_create_simple_cmd(tokens);
			ft_insert_simple_cmd(head, current_cmd);
			pipe_line->simple_cmd_count += 1;
		}
	}
	pipe_line->child = head;
	return pipe_line;
}

static t_command_list *ft_create_ast(t_token *tokens_list)
{
	t_command_list	*head;
	t_pipe_line		*current_pipeline;
	int				pipe_count;

	pipe_count = 0;
	head = init_cmd_list();
	current_pipeline = NULL;
	while (tokens_list->type != NEWLINE)
	{
		if (head->childs == NULL)
		{
			head->childs = ft_create_pieline(&tokens_list);
			head->pipe_line_count += 1;
		}
		else if (tokens_list->type == SEMI)
		{
			tokens_list = tokens_list->next;
			current_pipeline = head->childs;
			while (current_pipeline->next != NULL)
				current_pipeline = current_pipeline->next;
			if (tokens_list->type != NEWLINE && tokens_list->type != SEMI)
			{
				current_pipeline->next = ft_create_pieline(&tokens_list);
				head->pipe_line_count += 1;
			}
		}
		else
			tokens_list = tokens_list->next;
	}
	return (head);
}

t_command_list	*ft_parser(t_token *tokens_list, int *status)
{
	t_command_list	*command_list;

	command_list = NULL;
	print_tokens(tokens_list);
	write(1, RED, ft_strlen(RED));
	if (!ft_check_syntax(tokens_list, status))
	{
		command_list = ft_create_ast(tokens_list);
		ft_destoy_token_list(tokens_list);
	}
	return (command_list);
}
