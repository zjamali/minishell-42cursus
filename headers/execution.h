/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 15:45:53 by mbari             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2021/03/29 16:54:56 by zjamali          ###   ########.fr       */
=======
/*   Updated: 2021/03/29 19:09:50 by mbari            ###   ########.fr       */
>>>>>>> 36e66ce6ec332fa5509258bc7747e96450222a40
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
#define EXECUTION_H
#include "minishell.h"

t_env	*ft_create_node(char *name, char *value);
void    ft_add_to_list(t_env **head, t_env *newnode);
void	ft_delete_from_list(t_env **head, char *name);
void	ft_replaceit(t_env **head, char *name, char *value);
void	ft_loop(t_args *args);
void	ft_echo(t_args *args);
void	ft_pwd(t_env **head);
void	ft_env(t_env **head);
void	ft_export(t_env **head, t_args *args);
int		ft_is_builtins(t_simple_cmd *cmd, t_env **head);
t_env	*ft_search_in_list(t_env **head, char *name);
int		ft_execute(t_command_list *cmd, t_env **head);
void	init_env(t_env **head, char **env);
int		ft_count_list(t_env **head);
char	**ft_list_to_arr(t_env **head);
#endif