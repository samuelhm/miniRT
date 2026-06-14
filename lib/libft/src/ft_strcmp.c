/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 16:10:23 by erigonza          #+#    #+#             */
/*   Updated: 2024/12/15 13:46:42 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (s1[i] && s1[i] == s2[i])
	{
		i++;
		if (!s2[i])
			return (-1);
	}
	while (s2[j] && s1[j] == s2[j])
	{
		j++;
		if (!s1[j])
			return (-1);
	}
	return (i - j);
}
