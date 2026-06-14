/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   three_renders.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 17:09:37 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/18 17:20:58 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	render_one(void *param)
{
	t_data		*data;
	uint32_t	**new_img;

	data = (t_data *)param;
	data->x = data->mlx->width;
	data->y = data->mlx->height;
	data->god = true;
	mlx_resize_image(data->img, (uint32_t)data->x, (uint32_t)data->y);
	new_img = render(data, 1);
	fill_image(data, (uint32_t *)data->img->pixels, new_img);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	free_image_all(data, new_img);
	data->render_sel = NULL;
	data->last_render = ONE;
}

void	render_fast(void *param)
{
	t_data		*data;
	uint32_t	**new_img;

	data = (t_data *)param;
	data->render_sel = NULL;
	data->last_render = FAST;
	data->x = data->mlx->width;
	data->y = data->mlx->height;
	data->god = true;
	mlx_delete_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, (uint32_t)data->x, (uint32_t)data->y);
	new_img = console_render(data);
	fill_image(data, (uint32_t *)data->img->pixels, new_img);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	mlx_set_instance_depth(&data->img->instances[0], 1);
	free_image_all(data, new_img);
	run_console(data);
}

void	update_render(void *param)
{
	t_data		*data;
	uint32_t	**new_img;
	uint32_t	**avrg;
	t_ll		time;

	time = current_timestamp();
	data = (t_data *)param;
	data->x = data->mlx->width;
	data->y = data->mlx->height;
	data->god = true;
	mlx_resize_image(data->img, (uint32_t)data->x, (uint32_t)data->y);
	if (!data->img_last)
		data->img_last = render(data, 0);
	if (!data->img->enabled)
		data->img->enabled = true;
	new_img = render(data, 0);
	avrg = average_samples(data, (uint32_t **)data->img_last, new_img);
	fill_image(data, (uint32_t *)data->img->pixels, avrg);
	free_image_all(data, avrg);
	free_image_all(data, new_img);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	time = current_timestamp() - time;
	data->last_render = UPDATE;
}
