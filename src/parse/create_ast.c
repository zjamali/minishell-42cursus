/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 18:14:53 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:26:59 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

t_pipe_line	*ft_init_pipe_line(void)
{
	t_pipe_line		*pipe_line;

	pipe_line = (t_pipe_line *)malloc(sizeof(t_pipe_line));
	if (!pipe_line)
		return (NULL);
	pipe_line->next = NULL;
	pipe_line->child = NULL;
	pipe_line->simple_cmd_count = 0;
	return (pipe_line);
}

t_pipe_line	*reset_pipe_line_head(t_pipe_line **pipe_line, t_simple_cmd *head)
{
	(*pipe_line)->child = head;
	return (*pipe_line);
}

t_pipe_line	*ft_create_pieline(t_token **tokens)
{
	t_pipe_line		*pipe_line;
	t_simple_cmd	*head;
	t_simple_cmd	*current_cmd;

	pipe_line = ft_init_pipe_line();
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
				break ;
			(*tokens) = (*tokens)->next;
			current_cmd = ft_create_simple_cmd(tokens);
			ft_insert_simple_cmd(head, current_cmd);
			pipe_line->simple_cmd_count += 1;
		}
	}
	return (reset_pipe_line_head(&pipe_line, head));
}

void	ft_create_next_pipeline(t_token *tokens_list, t_command_list **head)
{
	t_pipe_line		*current_pipeline;

	current_pipeline = NULL;
	current_pipeline = (*head)->childs;
	while (current_pipeline->next != NULL)
		current_pipeline = current_pipeline->next;
	if (tokens_list->type != NEWLINE && tokens_list->type != SEMI)
	{
		current_pipeline->next = ft_create_pieline(&tokens_list);
		(*head)->pipe_line_count += 1;
	}
}

t_command_list	*ft_create_ast(t_token *tokens_list)
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
			ft_create_next_pipeline(tokens_list, &head);
		}
		else
			tokens_list = tokens_list->next;
	}
	return (head);
}
