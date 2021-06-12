/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 15:45:53 by mbari             #+#    #+#             */
/*   Updated: 2021/06/12 16:07:44 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
typedef struct s_mini
{
	int	*fd;
	int	red_fd[2];
	int	*pid;
	int	flag;
	int	ret;
	int	simple_cmd_count;
}				t_mini;

int		ft_isitn(char *arg);
int		ft_pwd(t_env **head);
int		ft_env(t_env **head);
void	ft_loop(t_args *args);
int		ft_exit(t_args *args);
int		ft_echo(t_args *args);
void	ft_do_backups(int flag);
int		ft_check_exit(char *arg);
void	ft_check_digit(char	*arg);
void	ft_free_list(char **list);
void	ft_swap_nodes(t_env	*temp);
void	ft_signal_handler(int sig);
int		ft_check_errors(char *dir);
t_env	*ft_copy_list(t_env **head);
t_env	*ft_sort_list(t_env **head);
int		ft_count_list(t_env **head);
int		ft_checkargs(t_args **args);
void	ft_destroy_env_list(t_env *head);
int		ft_count_args(t_args **args);
int		ft_print_export(t_env **head);
char	**ft_list_to_arr(t_env **head);
int		ft_cd(t_args *args, t_env **head);
char	**ft_args_to_arr(t_simple_cmd *cmd);
int		ft_file_err(t_redirection *redirect);
int		ft_unset(t_args *args, t_env **head);
char	*ft_join_path(char *path, char *cmd);
int		ft_export(t_env **head, t_args *args);
void	ft_init_env(t_env **head, char **env);
void	ft_add_var(t_env **head, t_args *args);
int		ft_change_dir(t_env **head, char *dir);
t_env	*ft_create_node(char *name, char *value);
int		ft_exec(t_simple_cmd *cmd, t_env **head);
int		ft_execute(t_pipe_line *cmd, t_env **head);
int		ft_close_fd(t_pipe_line *cmd, t_mini *mini);
t_env	*ft_search_in_list(t_env **head, char *name);
void	ft_add_to_list(t_env **head, t_env *newnode);
int		ft_find_file(t_simple_cmd *cmd, t_env **head);
void	ft_delete_from_list(t_env **head, char *name);
int		ft_check_path(t_simple_cmd *cmd, t_env **head);
int		ft_less_than(int *fd, t_redirection *redirect);
int		ft_file_check(t_simple_cmd *cmd, t_env **head);
int		ft_put_err(char *input, char *message, int ret);
int		ft_is_builtins(t_simple_cmd *cmd, t_env **head);
int		ft_great_than(int *fd, t_redirection *redirect);
int		ft_double_great(int *fd, t_redirection *redirect);
int		ft_create_fd_pipe(t_pipe_line *cmd, t_mini *mini);
void	ft_replaceit(t_env **head, char *name, char *value);
int		ft_replace_pwd(t_env **head, t_env *temp, char *dir);
int		ft_pipe(t_mini *mini, t_pipe_line *cmd, t_env **head);
int		ft_redirection(t_mini *mini, t_redirection *redirect);
void	ft_execve(t_simple_cmd *cmd, char **args, char **list);
void	ft_add_or_replace(t_env **head, char **split, int join);
int		ft_delete_first_node(t_env **head, t_env *temp, char *name);
void	ft_exec_file(t_simple_cmd *cmd, t_env **head, char *path, int *status);
void	ft_pipe_fork(t_simple_cmd *cmd, t_env **head, t_mini *mini,
			int *command);

#endif