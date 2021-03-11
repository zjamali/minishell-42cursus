/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 22:39:57 by zjamali           #+#    #+#             */
/*   Updated: 2019/11/09 15:43:41 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strrchr(const char *s, int c)
{
	int		i;

	i = (int)ft_strlen(s);
	if (s[i] == c)
		return ((char*)(s + i));
	while (i >= 0)
	{
		if (s[i] == c)
			return ((char*)(s + i));
		i--;
	}
	return (NULL);
}
