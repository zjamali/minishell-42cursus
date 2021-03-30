/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/19 20:28:44 by mbari             #+#    #+#             */
/*   Updated: 2021/03/30 17:07:25 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	if (s1 == NULL)
		return ft_strdup(s2);
	if (s2 == NULL)
		return ft_strdup(s1);
	char *join;
	if (!(join = malloc((sizeof(char)*(ft_strlen(s1)+ ft_strlen(s2) + 1)))))
		return NULL;
	int i = 0;
	int j = 0;
	while (s1[i])
	{
		join[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		join[j] = s2[i];
		j++;
		i++;
	}
	join[j] = '\0';
	return join;
}
