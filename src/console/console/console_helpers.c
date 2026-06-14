/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 19:41:14 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 12:27:03 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

mlx_image_t	*create_button(mlx_t *mlx, const char *label, int x, int y)
{
	mlx_image_t	*button;

	button = NULL;
	if (!mlx || !label)
		return (NULL);
	button = mlx_put_string(mlx, label, x, y);
	if (!button)
	{
		fprintf(stderr, "Failed to create button with label: %s\n", label);
		return (NULL);
	}
	return (button);
}

mlx_image_t	*create_menu_background(t_data *data)
{
	mlx_image_t	*background;
	int			x;
	int			y;
	uint32_t	*pixels;

	background = NULL;
	if (!data || !data->mlx)
		return (NULL);
	background = mlx_new_image(data->mlx, BG_WITH, data->mlx->height);
	if (!background)
		return (NULL);
	pixels = (uint32_t *)background->pixels;
	y = -1;
	while (++y < data->mlx->height)
	{
		x = -1;
		while (++x < BG_WITH)
		{
			if (y >= 65)
				pixels[y * BG_WITH + x] = 0x80000000;
			else
				pixels[y * BG_WITH + x] = 0xFF000000;
		}
	}
	return (background);
}

void	set_background(t_data *data)
{
	int	width;

	width = data->mlx->width - BG_WITH;
	if (data->console->background)
	{
		mlx_delete_image(data->mlx, data->console->background);
		data->console->background = NULL;
	}
	if (!data->console->background)
		data->console->background = create_menu_background(data);
	if (!data->console->background)
	{
		fprintf(stderr, "Failed to create menu background\n");
		return ;
	}
	mlx_image_to_window(data->mlx, data->console->background, width, 0);
	mlx_set_instance_depth(&data->console->background->instances[0], 2);
}

mlx_image_t	*put_str(mlx_t *mlx, const char *str, int x, int y)
{
	return (mlx_put_string(mlx, str, x, y));
}
