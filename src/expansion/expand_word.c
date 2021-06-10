/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:07:29 by zjamali           #+#    #+#             */
/*   Updated: 2021/06/10 20:25:14 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"
#include "../../headers/execution.h"

int	check_exiting_of_qoutes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			return (1);
		}
		else if (str[i] == '\"')
			return (2);
		i++;
	}
	return (0);
}

void	ft_expand_squence_of_dollar_sign(char *str, int *i, char **expanded)
{
	char	*tmp1;
	char	*tmp;

	tmp1 = *expanded;
	tmp = ft_substr(str, *i, 2);
	*expanded = ft_strjoin(*expanded, tmp);
	free(tmp1);
	free(tmp);
	*i += 2;
}

void	ft_replace_env_by_value(char *str, int *i, char **expanded,
		char **env_value)
{
	char	*tmp;

	tmp = *expanded;
	*expanded = ft_strjoin(*expanded, *env_value);
	free(*env_value);
	free(tmp);
	if (str[*i] == '$')
		(*i)++;
	if (str[*i] == '_')
		(*i)++;
	else
	{
		while (ft_isalpha(str[*i]) || ft_isalnum(str[*i])
			|| str[*i] == '_')
			(*i)++;
	}	
}

void	ft_skip_characters_env_not_exist(char *str, int *i)
{
	if (str[*i] == '$')
		(*i)++;
	while (ft_isalpha(str[*i]) || ft_isalnum(str[*i])
		|| str[*i] == '_')
		(*i)++;
}

void	ft_expande_word(char **string, t_env **env_list, char **last_env,
		int redirection)
{
	t_expansion	expd;
	int			i;

	i = 0;
	expd.word = *string;
	expd.expanded = NULL;
	ft_replace_tilde(&expd.word);
	while (expd.word[i])
	{
		if (expd.word[i] == '\\')
			ft_expand_backslashes(expd.word, &i, &expd.expanded);
		else if (expd.word[i] == '\'')
			ft_remove_single_quotes(expd.word, &i, &expd.expanded);
		else if (expd.word[i] == '"')
			expd.expanded = ft_expand_double_quotes(expd, &i, env_list,
					last_env);
		else if (expd.word[i] == '$')
			ft_expand_env_variable(&expd, &i, env_list, last_env);
		else
			ft_get_charachter(expd.word, &i, &expd.expanded);
	}
	check_word_expand_redtion(redirection, &expd.expanded, &expd.word, string);
}
