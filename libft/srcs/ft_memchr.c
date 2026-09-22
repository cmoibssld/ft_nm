/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 14:55:59 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 13:21:24 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*bs;
	unsigned char	bc;
	size_t			i;

	i = 0;
	bs = (unsigned char *)s;
	bc = (unsigned char)c;
	while (i < n)
		if (bs[i++] == bc)
			return (bs + (i - 1));
	return (NULL);
}
