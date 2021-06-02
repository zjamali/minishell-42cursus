/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_get_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/02 09:28:52 by zjamali           #+#    #+#             */
/*   Updated: 2021/05/02 10:56:33 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

/*
* begining is bach_slash "\"
* begining is single_quoting '
* begining is double_quoting "
* no quoting
*/
static int	check_the_beginning_of_word(int c)
{
	if (c == 92)
		return (1);
	else if (c == 39)
		return (2);
	else if (c == 34)
		return (3);
	else
		return (0);
}

static void	add_token_and_increament_index(t_token *tokens_list, char *word,
			int *table, int j)
{
	add_token(tokens_list, WORD, word, table[3]);
	table[1] = j;
	table[3]++;
}

void	ft_get_word(t_token *tokens_list, char *line, int *table)
{
	int		quoting;
	char	*word;
	int		j;

	j = 0;
	quoting = -1;
	word = NULL;
	j = table[1];
	while (line[j])
	{
		quoting = check_the_beginning_of_word(line[j]);
		if (quoting == 0)
		{
			word = ft_get_words(line, &j, word, &quoting);
			if (ft_strchr("\t ><|;", line[j]))
				break ;
		}
		else if (quoting > 0)
		{
			word = ft_get_words(line, &j, word, &quoting);
			if (line[j] == ' ' || line[j] == '\t')
				break ;
		}
	}
	add_token_and_increament_index(tokens_list, word, table, j);
}
