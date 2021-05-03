/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 15:45:53 by mbari             #+#    #+#             */
/*   Updated: 2021/05/02 15:49:05 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H
#include "minishell.h"


typedef struct s_mini
{
	int 	*fd;
	int		red_fd[2];
	int		*pid;
	int 	flag;
	int 	ret;
}				t_mini;

t_env	*ft_create_node(char *name, char *value);
void    ft_add_to_list(t_env **head, t_env *newnode);
void	ft_delete_from_list(t_env **head, char *name);
void	ft_replaceit(t_env **head, char *name, char *value);
void	ft_loop(t_env **head, t_args *args);
int		ft_echo(t_env **head, t_args *args);
int		ft_cd(t_args *args, t_env **head);
int		ft_pwd();
int		ft_env(t_env **head);
int		ft_unset(t_args *args, t_env **head);
int		ft_export(t_env **head, t_args *args);
int		ft_exit(t_args *args);
int		ft_is_builtins(t_simple_cmd *cmd, t_env **head);
t_env	*ft_search_in_list(t_env **head, char *name);
int		ft_execute(t_pipe_line *cmd, t_env **head);
void	init_env(t_env **head, char **env);
int		ft_count_list(t_env **head);
char	**ft_list_to_arr(t_env **head);
t_env	*ft_sort_list(t_env **head);
int		ft_put_err(char *input, char *message, int ret);
void	do_backups(int flag);
int		ft_less_than(int *fd, t_redirection *redirect);
int		ft_great_than(int *fd, t_redirection *redirect);
int 	ft_double_great(int *fd, t_redirection *redirect);
int		ft_redirection(t_mini *mini, t_redirection *redirect);
#endif