/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 17:13:30 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 12:28:48 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;
	char	*bres;
	size_t	i;

	res = malloc(nmemb * size);
	if (!res)
		return (NULL);
	bres = (char *)res;
	i = 0;
	while (i < nmemb * size)
		bres[i++] = 0;
	return (res);
}
