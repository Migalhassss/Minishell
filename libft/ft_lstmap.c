/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sezequie <sezequie@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:49:53 by micarrel          #+#    #+#             */
/*   Updated: 2023/09/08 15:17:30 by sezequie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*aux_list;
	t_list	*hold;

	if (!lst)
		return (0);
	aux_list = 0;
	while (lst)
	{
		hold = ft_lstnew(f(lst->content));
		if (!hold)
		{
			ft_lstclear(&aux_list, del);
			return (0);
		}
		ft_lstadd_back(&aux_list, hold);
		lst = lst->next;
	}
	return (aux_list);
}
