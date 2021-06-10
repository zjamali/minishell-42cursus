/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/21 20:32:35 by mbari             #+#    #+#             */
/*   Updated: 2021/06/06 09:49:42 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/execution.h"

int	ft_checkargs(t_args **args)
{
	t_args	*temp;
	char	*error;
	char	*join;
	int		ret;

	temp = *args;
	ret = 0;
	while (temp != NULL)
	{
		if (temp->value == NULL)
			ret = ft_put_err("export:", " `': not a valid identifier", 1);
		else if (!(ft_isalpha(temp->value[0]) || temp->value[0] == '_'))
		{
			join = ft_strjoin(temp->value, "': not a valid identifier");
			error = ft_strjoin(" `", join);
			free (join);
			ret = ft_put_err("export:", error, 1);
			free (error);
		}
		temp = temp->next;
	}
	return (ret);
}

int	ft_print_export(t_env **head)
{
	t_env	*newnode;
	t_env	*sort_list;

	sort_list = ft_sort_list(head);
	newnode = sort_list;
	while (newnode != NULL)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(newnode->name, 1);
		if (newnode->value != NULL)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(newnode->value, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		newnode = newnode->next;
	}
	ft_destroy_env_list(sort_list);
	return (0);
}

void	ft_add_or_replace(t_env **head, char **split, int join)
{
	t_env	*newnode;
	char	*join_them;

	newnode = ft_search_in_list(head, split[0]);
	if (newnode != NULL)
	{
		if (join == YES)
		{
			join_them = ft_strjoin(newnode->value, split[1]);
			free(split[1]);
			split[1] = join_them;
		}
		if (split[1] != NULL)
			ft_replaceit(head, split[0], split[1]);
	}
	else
	{
		newnode = ft_create_node(split[0], split[1]);
		ft_add_to_list(head, newnode);
	}
}

void	ft_add_var(t_env **head, t_args *args)
{
	char	**split;
	int		join;

	join = NO;
	split = my_split(args->value);
	if (split[0][ft_strlen(split[0]) - 1] == '+')
	{
		split[0][ft_strlen(split[0]) - 1] = '\0';
		join = YES;
	}
	ft_add_or_replace(head, split, join);
	args = args->next;
	free(split[0]);
	free(split[1]);
	free(split);
}

int	ft_export(t_env **head, t_args *args)
{
	int		ret;

	ret = 0;
	if (args == NULL)
		ret = ft_print_export(head);
	ret = ft_checkargs(&args);
	while (args != NULL)
	{
		if (args->value != NULL && (ft_isalpha(args->value[0])
				|| args->value[0] == '_'))
			ft_add_var(head, args);
		args = args->next;
	}
	return (ret);
}
