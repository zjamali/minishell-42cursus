/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 19:29:05 by zjamali           #+#    #+#             */
/*   Updated: 2019/11/09 15:35:41 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void		*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*psrc;
	char	*pdst;

	psrc = (char *)src;
	pdst = (char *)dst;
	if (!psrc && !pdst)
		return (NULL);
	if (src < dst)
	{
		while (len > 0)
		{
			len--;
			pdst[len] = psrc[len];
		}
	}
	else
		ft_memcpy(dst, src, len);
	return (dst);
}
