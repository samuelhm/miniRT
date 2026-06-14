/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rays_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:34:39 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 17:43:46 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	free_rays(t_ray **rays, int rows)
{
	int	i;

	i = -1;
	while (++i < rows)
		free(rays[i]);
	free(rays);
}
