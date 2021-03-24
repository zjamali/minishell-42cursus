/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 22:12:29 by mbari             #+#    #+#             */
/*   Updated: 2019/11/19 16:50:43 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		count_words(char const *s, char c)
{
	int			count;

	count = 0;
	while (*s && *s == c)
		s++;
	while (*s)
	{
		if (*s != c && (*(s + 1) == c || *(s + 1) == '\0'))
			count++;
		s++;
	}
	return (count);
}

static char		*word_len(char const *str, char c)
{
	char		*word;
	int			i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	word = (char *)malloc(sizeof(char) * (i + 1));
	i = 0;
	while (str[i] && str[i] != c)
	{
		word[i] = (char)str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void		freeall(int i, char **arr)
{
	int			j;

	j = -1;
	while (j++ <= i)
		free(arr[j]);
	free(arr);
}

char			**ft_split(char const *s, char c)
{
	int			i;
	int			len_w;
	char		**arr;

	if (!s)
		return (NULL);
	len_w = count_words(s, c);
	if (!(arr = (char **)malloc(sizeof(char*) * (len_w + 1))))
		return (NULL);
	i = 0;
	while (i < len_w)
	{
		while (*s && *s == c)
			s++;
		arr[i] = word_len(s, c);
		if (!arr[i])
			freeall(i, arr);
		while (*s && *s != c)
			s++;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
