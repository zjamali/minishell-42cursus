/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 15:45:53 by mbari             #+#    #+#             */
/*   Updated: 2021/03/28 12:39:08 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H
#include "minishell.h"
#include "../src/libft/libft.h"
#include "../headers/get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

typedef struct s_env
{
	char *name;
	char *value;
	struct s_env *next;
}				t_env;

t_env	*ft_create_node(char *name, char *value);
void    ft_add_to_list(t_env **head, t_env *newnode);
void	ft_delete_from_list(t_env **head, char *name);
void	ft_replaceit(t_env **head, char *name, char *value);
void	ft_loop(t_args *args);
void	ft_echo(t_args *args);
void	ft_pwd(t_env **head);
void	ft_env(t_env **head);
void	ft_export(t_env **head, t_args *args);
void	ft_is_builtins(t_simple_cmd *cmd, t_env **head);
t_env	*ft_search_in_list(t_env **head, char *name);
int		ft_execute(t_command_list *cmd, t_env **head);
void	init_env(t_env **head, char **env);

#endif