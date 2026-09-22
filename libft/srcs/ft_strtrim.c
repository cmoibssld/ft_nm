/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 18:53:05 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/27 16:35:56 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static	int	isinset(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
		if (c == set[i++])
			return (1);
	return (0);
}

static int	get_start(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (isinset(s1[i], set))
		i++;
	return (i);
}

static int	get_end(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (s1[i])
		i++;
	i--;
	while (isinset(s1[i], set))
		i--;
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*res;

	i = get_start(s1, set);
	j = get_end(s1, set);
	if (j >= i && (j != 0 || i != 0))
		res = (char *)malloc((j - i + 2) * sizeof(char));
	else
		res = (char *)malloc(1 * sizeof(char));
	if (!res)
		return (NULL);
	k = 0;
	if (!s1[i] || j < i)
	{
		res[0] = '\0';
		return (res);
	}
	while (i <= j)
		res[k++] = s1[i++];
	res[k] = '\0';
	return (res);
}
