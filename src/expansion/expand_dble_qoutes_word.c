/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dble_qoutes_word.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 19:35:26 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:10:13 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

void	ft_expand_underscore_in_double_qts(char *str, int i, char **env_value,
		char *last_argmnt)
{
	if (str[i + 1] == '_' && str[i + 2] == '"' && last_argmnt)
	{
		free(*env_value);
		*env_value = NULL;
		*env_value = ft_strdup(last_argmnt);
	}
}

void	ft_expand_env_variable_in_dble_qte(t_expansion *expd, int *j,
		t_env **env_list, char **last_env)
{
	if (expd->word[*j + 1] == '$')
		ft_expand_squence_of_dollar_sign(expd->word, j, &(expd->expanded));
	else
	{
		expd->tmp = get_env_variable_value(expd->word + *j, env_list);
		if (expd->tmp)
		{
			ft_expand_underscore_in_double_qts(expd->word, *j, &(expd->tmp),
				last_env[1]);
			ft_replace_env_by_value(expd->word, j, &(expd->expanded),
				&(expd->tmp));
		}
		else if (j == 0 || expd->word[*j - 1] != '$')
			ft_expande_special_param_in_dble_g(expd->word, j,
				&(expd->expanded), last_env[0]);
		else
			ft_skip_characters_non_env_variable(expd->word, j,
				&(expd->expanded), 1);
	}
}

void	ft_expand_backslashes_in_dble_qts(char *str, int *i, char **expanded)
{
	int		j;
	char	*tmp1;
	char	*tmp;

	j = *i;
	if (ft_strchr("$\"\\\n`", str[j + 1]))
	{
		tmp = *expanded;
		tmp1 = ft_substr(str, j + 1, 1);
		*expanded = ft_strjoin(*expanded, tmp1);
		j += 2;
		free(tmp);
		free(tmp1);
	}
	else
	{
		tmp = *expanded;
		tmp1 = ft_substr(str, j, 2);
		*expanded = ft_strjoin(*expanded, tmp1);
		j += 2;
		free(tmp);
		free(tmp1);
	}
	*i = j;
}

char	*ft_remove_double_quotes(char *word, int *i, t_env **env_list,
		char **last_env)
{
	t_expansion	expd;
	int			j;

	expd.expanded = NULL;
	j = *i + 1;
	expd.word = word;
	while (expd.word[j] != '"')
	{
		if (expd.word[j] == '\\')
			ft_expand_backslashes_in_dble_qts(expd.word, &j, &expd.expanded);
		else
		{
			if (expd.word[j] == '$')
				ft_expand_env_variable_in_dble_qte(&expd, &j, env_list,
					last_env);
			else
				ft_get_charachter(expd.word, &j, &expd.expanded);
		}
	}
	j++;
	*i = j;
	return (expd.expanded);
}

char	*ft_expand_double_quotes(t_expansion expd, int *i, t_env **env_list,
		char **last_env)
{
	expd.tmp1 = expd.expanded;
	expd.tmp = ft_remove_double_quotes(expd.word, i, env_list, last_env);
	if (expd.tmp)
	{
		expd.expanded = ft_strjoin(expd.expanded, expd.tmp);
		free(expd.tmp);
		free(expd.tmp1);
	}
	return (expd.expanded);
}
