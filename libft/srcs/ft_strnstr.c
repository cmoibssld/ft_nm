/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:19:17 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/27 16:26:19 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	j;
	size_t	i;

	if (!*needle)
		return ((char *)haystack);
	if (!*haystack || !len)
		return (NULL);
	i = 0;
	while (i < len && *haystack)
	{
		j = 0;
		while ((needle[j] == haystack[j]) && ((i + j) < len))
			if (!needle[++j])
				return ((char *)haystack);
		haystack++;
		i++;
	}
	return (NULL);
}
