/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:42:55 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/27 16:26:55 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	k;
	char	*res;

	i = ft_strlen(s) + 1;
	k = (char)c;
	res = (char *)s;
	while (i-- > 0)
		if (s[i] == k)
			return (res + i);
	return (NULL);
}
