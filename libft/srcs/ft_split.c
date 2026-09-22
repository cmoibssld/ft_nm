/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 11:16:14 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/24 09:12:07 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int	not_c(char const *s, char c)
{
	size_t	i;
	int		res;

	i = 0;
	res = 0;
	while (s[i])
		if (s[i++] != c)
			res++;
	return (res);
}

static int	token(char const *s, char c)
{
	int		res;

	res = 0;
	while (*s && *s == c)
		s++;
	while (*s)
	{
		if (*s == c)
		{
			if (!*(s + 1))
				break ;
			if (*(s + 1) != c)
				res++;
		}
		s++;
	}
	return (res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	char	*str_tmp;
	size_t	i;
	int		col;

	col = token(s, c) + 1 + 1;
	res = malloc(col * sizeof(char *) + (not_c(s, c) + col - 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	str_tmp = (char *)(res + col);
	while (*s)
	{
		while (*s == c)
			s++;
		if (!*s)
			break ;
		res[i++] = str_tmp;
		while (*s && *s != c)
			*str_tmp++ = *s++;
		*str_tmp++ = '\0';
	}
	res[i] = NULL;
	return (res);
}
