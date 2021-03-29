/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbari <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 20:28:44 by mbari             #+#    #+#             */
/*   Updated: 2019/11/18 16:50:06 by mbari            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t			i;
	size_t			j;
	unsigned char	*join;

	if (s1 != NULL && s2 != NULL)
	{
		join = (unsigned char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
		if (!join)
			return (NULL);
		i = -1;
		j = 0;
		while (s1[++i] != '\0')
			join[i] = s1[i];
		while (s2[j] != '\0')
			join[i++] = s2[j++];
		join[i] = '\0';
		return ((char *)join);
	}
	return (NULL);
}
