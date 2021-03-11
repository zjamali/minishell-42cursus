/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 16:31:40 by zjamali           #+#    #+#             */
/*   Updated: 2019/11/13 17:34:35 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*strstart(char *str, char *set)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (str[i] && set[j])
	{
		if (str[i] == set[j])
		{
			i++;
			j = 0;
		}
		else
			j++;
	}
	return (&str[i]);
}

int		strend(char *str, char *set)
{
	int		i;
	int		j;

	i = ft_strlen(str);
	j = 0;
	while (i > 0 && set[j])
	{
		if (str[i - 1] == set[j])
		{
			i--;
			j = 0;
		}
		else
			j++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char		*tab;
	char		*delt;
	char		*ptr;
	char		*string;
	size_t		i;

	tab = (char *)s1;
	delt = (char *)set;
	if (!s1 || !set)
		return (NULL);
	string = strstart(tab, delt);
	i = strend(string, delt);
	ptr = malloc(sizeof(char) * i + 1);
	if (!ptr)
		return (NULL);
	ft_memcpy(ptr, string, i);
	ptr[i] = '\0';
	return (ptr);
}
