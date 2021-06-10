/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special_params.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:48:43 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:09:46 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

void	ft_expand_rest_of_specail_params(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 2);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
}

void	ft_expand_other_special_params(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	if (str[*i + 1] == '#')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("0");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	else if (str[*i + 1] == '-')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("himBH");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	else if (ft_strchr(":%=/!", str[*i + 1]))
		ft_expand_rest_of_specail_params(str, i, expanded);
	*i += 2;
}

void	ft_expand_back_slash(int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_strdup("$");
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	(*i)++;
}

void	ft_digit_env(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	if (str[*i + 1] == '0')
	{
		tmp1 = *expanded;
		tmp = ft_strdup("minishell");
		*expanded = ft_strjoin(*expanded, tmp);
		free(tmp1);
		free(tmp);
	}
	*i += 2;
}

void	ft_expande_special_params(char *str, int *i, char **expanded,
		char *exit_status)
{
	if (str[*i + 1] != '"')
	{
		if (str[*i + 1] && (ft_isdigit(str[*i + 1])
				|| ft_strchr("!:#%@-*=/\\", str[*i + 1])))
		{
			if (ft_isdigit(str[*i + 1]))
				ft_digit_env(str, i, expanded);
			else if (ft_strchr("\\", str[*i + 1]))
				ft_expand_back_slash(i, expanded);
			else
				ft_expand_other_special_params(str, i, expanded);
		}
		else if (str[*i + 1] == '?')
			ft_expand_exit_status(i, expanded, exit_status);
		else if (str[*i + 1])
			ft_skip_characters_env_not_exist(str, i);
		else
			get_dollar_sign(str, i, expanded);
	}
	else
		(*i)++;
}
