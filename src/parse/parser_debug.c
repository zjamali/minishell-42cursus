/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/31 18:39:43 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/31 18:40:46 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

void	ft_print_simple_cmd(t_simple_cmd *cmd)
{
	t_redirection *tmp;
	t_args *tmp1;

	tmp1 = cmd->args;
	tmp = cmd->redirections;
	ft_putstr_fd(YELLOW, 1);
	ft_putstr_fd("(", 1);
	ft_putstr_fd("cmd -> ", 1);
	ft_putstr_fd("{", 1);
	if (cmd->command)
		write(1, cmd->command, ft_strlen(cmd->command));
	ft_putstr_fd("}", 1);
	ft_putstr_fd("\t", 1);
	ft_putstr_fd("inside quotes  -> ", 1);
	ft_putnbr_fd(cmd->inside_quotes, 1);
	ft_putstr_fd(")", 1);
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("args -> ", 1);
	while (tmp1)
	{
		ft_putstr_fd("[", 1);
		ft_putstr_fd("{", 1);
		ft_putstr_fd(tmp1->value, 1);
		ft_putstr_fd("}", 1);
		ft_putstr_fd("\t", 1);
		ft_putstr_fd("inside quotes -> ", 1);
		ft_putnbr_fd(tmp1->inside_quotes, 1);
		ft_putstr_fd("] ", 1);
		tmp1 = tmp1->next;
	}
	ft_putstr_fd("\n", 1);

	ft_putstr_fd("redirs ->", 1);
	while (tmp != NULL)
	{
		ft_putstr_fd("(", 1);
		ft_putnbr_fd(tmp->type, 1);
		ft_putstr_fd(" ", 1);
		ft_putstr_fd("{", 1);
		ft_putstr_fd(tmp->file_name, 1);
		ft_putstr_fd("}", 1);
		ft_putstr_fd("\t", 1);
		ft_putstr_fd("inside quotes -> ", 1);
		ft_putnbr_fd(tmp->inside_quotes, 1);
		ft_putstr_fd(") ", 1);
		tmp = tmp->next;
	}
	ft_putstr_fd("\n", 1);
}

void	ft_print_pipeline_cmd(t_pipe_line *pipe_line)
{
	t_simple_cmd	*tmp;
	int	i;

	i = 0;
	ft_putstr_fd("simple_cmd :", 1);
	ft_putnbr_fd(pipe_line->simple_cmd_count, 1);
	ft_putstr_fd("\n", 1);
	tmp = pipe_line->child;
	while (tmp != NULL)
	{
		ft_putstr_fd(PURPLE, 1);
		ft_putstr_fd("pipe : ", 1);
		ft_putnbr_fd(i, 1);
		ft_putstr_fd("\n", 1);
		ft_print_simple_cmd(tmp);
		tmp = tmp->next;
		i++;
	}
}

void	ft_print_cmd_list(t_command_list *cmd_list)
{
	t_pipe_line *tmp;

	tmp = cmd_list->childs;
	int i = 0;
	ft_putstr_fd("\n", 1);
	ft_putstr_fd("pipeline_count :", 1);
	ft_putnbr_fd(cmd_list->pipe_line_count, 1);
	while (tmp)
	{
		ft_putstr_fd("\n", 1);
		ft_putstr_fd(BLUE, 1);
		ft_putstr_fd("pipeline  : ", 1);
		ft_putnbr_fd(i, 1);
		ft_putstr_fd("\n", 1);
		ft_print_pipeline_cmd(tmp);
		ft_putstr_fd("\n", 1);
		tmp = tmp->next;
		i++;
	}
}
