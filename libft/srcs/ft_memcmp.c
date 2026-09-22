/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:10:48 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 12:09:40 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*bs1;
	unsigned char	*bs2;
	size_t			i;

	i = 0;
	bs1 = (unsigned char *)s1;
	bs2 = (unsigned char *)s2;
	while (i++ < n)
		if (bs1[i - 1] != bs2[i - 1])
			return (bs1[i - 1] - bs2[i - 1]);
	return (0);
}
