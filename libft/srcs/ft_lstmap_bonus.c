/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlepany <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 18:19:09 by jlepany           #+#    #+#             */
/*   Updated: 2025/10/23 13:01:36 by jlepany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	content = f(lst->content);
	new_node = ft_lstnew(content);
	if (!new_node)
		return (del(content), NULL);
	new_lst = new_node;
	lst = lst->next;
	while (lst)
	{
		content = f(lst->content);
		new_node->next = ft_lstnew(content);
		if (!new_node->next)
			return (del(content), ft_lstclear(&new_lst, del), NULL);
		new_node = new_node->next;
		lst = lst->next;
	}
	return (new_lst);
}
