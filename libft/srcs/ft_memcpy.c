/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 15:36:49 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 13:21:47 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	char	*bdest;
	char	*bsrc;

	if (dest == src)
		return (NULL);
	i = 0;
	bdest = (char *)dest;
	bsrc = (char *)src;
	while (i++ < n)
		bdest[i - 1] = bsrc[i - 1];
	return (dest);
}
