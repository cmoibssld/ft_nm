/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 12:14:29 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 12:29:09 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static int	count_digits(int n)
{
	int	i;

	i = 1 + (n < 0);
	while (n / 10)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static char	*recursive_completion(char *s, int n)
{
	if (n / 10)
		s = recursive_completion(s, n / 10);
	n %= 10;
	if (n < 0)
		n = -n;
	*s++ = n + '0';
	return (s);
}

char	*ft_itoa(int n)
{
	char	*res;

	res = ft_calloc(count_digits(n) + 1, sizeof(char));
	if (!res)
		return (NULL);
	if (n < 0)
	{
		res[0] = '-';
		recursive_completion(&res[1], n);
	}
	else if (n == 0)
		res[0] = '0';
	else
		recursive_completion(&res[0], n);
	return (res);
}
