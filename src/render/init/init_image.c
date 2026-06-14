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

//Memmory for the final image
uint32_t	**init_image_(t_data *data)
{
	uint32_t	**image;
	int			y;
	int			j;
	size_t		row_size;

	image = calloc(data->y, sizeof(uint32_t *));
	row_size = data->x * sizeof(uint32_t);
	if (!image)
		return (NULL);
	y = -1;
	while (++y < data->y)
	{
		image[y] = calloc(1, row_size);
		if (!image[y])
		{
			j = -1;
			while (++j < y)
				free(image[j]);
			free(image);
			return (NULL);
		}
	}
	return (image);
}
