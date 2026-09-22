/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:48:12 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/24 15:25:13 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

#include <string.h>

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dsize)
{
	size_t	i;
	size_t	dlen;
	size_t	slen;

	dlen = ft_strlen(dst);
	slen = ft_strlen(src);
	if (dlen >= dsize)
		return (dsize + slen);
	while (*dst)
		dst++;
	i = 0;
	while (*src && i + dlen + 1 < dsize)
		dst[i++] = *src++;
	dst[i] = '\0';
	return (dlen + slen);
}
