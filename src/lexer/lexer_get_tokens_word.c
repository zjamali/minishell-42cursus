/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens_word.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 10:51:03 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/31 13:24:14 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

static char	*get_no_quoting_word(char *line, int *i)
{
	char	*word;
	int		j;

	j = *i;
	while (line[j] && !ft_strrchr("\t '\"\\<>;|", line[j]))
		j++;
	word = ft_substr(line, *i, j - *i);
	*i = j;
	return (word);
}

static int	ft_count_backslashes(char *line, int j)
{
	int		k;
	int		back_slash_count;

	k = j - 1;
	back_slash_count = 0;
	while (line[k] == '\\')
	{
		back_slash_count++;
		k--;
	}
	return (back_slash_count);
}

static char	*get_double_quotes_word(char *line, int *i, int j)
{
	char	*word;
	int		back_slash_count;

	back_slash_count = 0;
	word = NULL;
	while (line[j])
	{
		if (line[j] == '"' && line[j - 1] != '\\')
			break ;
		else if (line[j] == '"' && line[j - 1] == '\\')
		{
			back_slash_count = ft_count_backslashes(line, j);
			if (back_slash_count % 2 == 0)
			{
				back_slash_count = 0;
				break ;
			}
			else
				back_slash_count = 0;
		}
		j++;
	}
	word = ft_substr(line, *i, j - *i + 1);
	*i = j + 1;
	return (word);
}

static char	*get_quoting_word(char *line, int *i, int quoting)
{
	int		j;
	char	*word;

	j = *i;
	word = NULL;
	if (quoting == 1)
	{
		word = ft_substr(line, *i, 2);
		*i += 2;
		return (word);
	}
	else if (quoting == 2)
	{
		j++;
		while (line[j] && line[j] != '\'')
			j++;
		word = ft_substr(line, *i, j - *i + 1);
		*i = j + 1;
		return (word);
	}
	else if (quoting == 3)
		return (get_double_quotes_word(line, i, ++j));
	return (word);
}

char	*ft_get_words(char *line, int *j, char *word, int *quoting)
{
	char	*tmp;
	char	*tmp1;

	if (*quoting == 0)
	{
		tmp = word;
		tmp1 = get_no_quoting_word(line, j);
		word = ft_strjoin(word, tmp1);
		free(tmp);
		free(tmp1);
	}
	else if (*quoting > 0)
	{
		tmp = word;
		tmp1 = get_quoting_word(line, j, *quoting);
		word = ft_strjoin(word, tmp1);
		free(tmp1);
		free(tmp);
		*quoting = -1;
	}
	return (word);
}
