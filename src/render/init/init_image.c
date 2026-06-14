/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:37:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/13 11:25:32 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"
#include "libvct.h"

void	init_mlx(t_data *data)
{
	data->mlx = mlx_init(data->x, data->y, "RT", true);
	if (!data->mlx)
		er(data, "Failed to initialize MLX42", NULL);
	data->img = mlx_new_image(data->mlx, data->x, data->y);
	if (!data->img)
	{
		mlx_terminate(data->mlx);
		er(data, "Failed to create image\n", NULL);
	}
}

uint32_t	*init_image_(t_data *data)
{
	uint32_t	*image;

	image = calloc((size_t)data->y * data->x, sizeof(uint32_t));
	return (image);
}
