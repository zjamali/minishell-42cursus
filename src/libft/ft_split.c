/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <mbari@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 22:12:29 by mbari             #+#    #+#             */
/*   Updated: 2021/04/04 19:13:52 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int		words(char *str, char c)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] == c && str[i])
			i++;
		if (str[i] && str[i] != c)
		{
			i++;
			j++;
		}
		while (str[i] && str[i] != c)
			i++;
	}
	return (j);
}

static void		*leak(char **spl, int j)
{
	j = j - 1;
	while (spl[j])
	{
		free(spl[j]);
		j--;
	}
	free(spl);
	return (NULL);
}

static int		carcts(char *str, char c)
{
	int			i;

	i = 0;
	while (str[i] && str[i] != c)
	{
		i++;
	}
	return (i);
}

static char		*allocandfill(char **tab, char *src, char c)
{
	int			i;
	int			j;
	int			k;

	j = 0;
	k = 0;
	while (src[k] == c)
		k++;
	while (j < words(src, c))
	{
		i = 0;
		if (!(tab[j] = malloc(sizeof(char) * (carcts(&src[k], c) + 1))))
			return (leak(tab, j));
		while (src[k] != c && src[k])
			tab[j][i++] = src[k++];
		tab[j][i] = '\0';
		while (src[k] == c && src[k])
			k++;
		j++;
	}
	tab[j] = NULL;
	return (*tab);
}

char			**ft_split(char const *s, char c)
{
	int			i;
	int			j;
	int			k;
	char		**tab;
	char		*str;

	k = 0;
	i = 0;
	j = 0;
	if (!s)
		return (NULL);
	str = (char *)s;
	tab = malloc(sizeof(char *) * (words(str, c) + 1));
	if (!tab)
		return (NULL);
	tab[j] = allocandfill(tab, str, c);
	return (tab);
}

char **my_split(char *str)
{
	int i;
	int j;
	char **s;
	
	i = 0;
	j = 0;
	s = (char **)malloc(sizeof(char) * 2);
	s[0] = (char *)malloc(sizeof(char) * carcts(str, '=') + 1);
	s[1] = (char *)malloc(sizeof(char) * (ft_strlen(str) - carcts(str, '=')) + 1);
	while (str[i] != '=' && str[i])
	{
		s[0][i] = str[i];
		i++;
	}
	ft_putchar_fd(str[i], 1);
	//i++;
	if (str[i] == '=' && str[i + 1] == '\0')
		s[1][0] = '\0';
	else if (str[i] == '\0')
		s[1] = NULL;
	else
		while (str[++i] != '\0')
			s[1][j++] = str[i];
	return (s);
}