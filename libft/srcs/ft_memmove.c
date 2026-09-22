/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:43:12 by jlepany           #+#    #+#             */
/*   Updated: 2024/11/18 12:44:18 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	temp[1];
	size_t			i;

	if (dest == src)
		return (dest);
	i = 0;
	if (dest >= src)
	{
		while (n > 0)
		{
			temp[0] = ((unsigned char *)src)[n - 1];
			((unsigned char *)dest)[n-- - 1] = temp[0];
		}
		return (dest);
	}
	while (i < n)
	{
		temp[0] = ((unsigned char *)src)[i];
		((unsigned char *)dest)[++i - 1] = temp[0];
	}
	return (dest);
}
