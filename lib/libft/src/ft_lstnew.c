/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/20 15:30:14 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/13 11:21:55 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*hal;

	hal = (t_list *)calloc(1, sizeof(*hal));
	if (!hal)
		return (NULL);
	hal->content = content;
	hal->next = NULL;
	return (hal);
}
