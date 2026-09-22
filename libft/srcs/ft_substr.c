/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:04:15 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/27 16:40:34 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static char	*nicemalloc(size_t i, unsigned int start, size_t len)
{
	char	*news;

	if (start > i || len == 0 || i == 0)
		news = (char *)malloc(1 * sizeof(char));
	else if (i - start > len)
		news = (char *)malloc((len + 1) * sizeof(char));
	else
		news = (char *)malloc((i - start + 1) * sizeof(char));
	return (news);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*news;
	size_t	i;

	i = ft_strlen(s);
	news = nicemalloc(i, start, len);
	if (!news)
		return (NULL);
	if (start > i || len == 0 || i == 0)
		len = 0;
	i = 0;
	while (i < len && s[start])
		news[i++] = s[start++];
	news[i] = '\0';
	return (news);
}
