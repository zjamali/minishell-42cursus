/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/11 15:07:04 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 18:07:47 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "../headers/execution.h"

void	show_prompt(void)
{
	write(1, GREEN, ft_strlen(GREEN));
	write(1, "minishell$ ", strlen("minishell$ "));
	write(1, RESET, ft_strlen(RESET));
}

t_simple_cmd	*ft_delete_empty_node_between(t_simple_cmd	**head,
		t_simple_cmd **temp)
{
	t_simple_cmd	*prev;

	prev = NULL;
	while (*temp != NULL)
	{
		while ((*temp) != NULL && ((*temp)->inside_quotes != 0
				|| ((*temp)->command && (*temp)->inside_quotes == 0)))
		{
			prev = (*temp);
			(*temp) = (*temp)->next;
		}
		if ((*temp) == NULL)
			return (*head);
		if (prev)
		{
			prev->next = (*temp)->next;
			ft_destroy_simple((*temp));
			(*temp) = prev->next;
		}
		else
			(*temp) = (*temp)->next;
	}
	return (*head);
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
	*head = *ft_delete_empty_node_between(&head, &temp);
	return (head);
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	ft_minishell(env);
	return (0);
}
