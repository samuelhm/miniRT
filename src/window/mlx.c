/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:58:38 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/24 11:05:27 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

bool	press_keyhook_normi(t_data *data, keys_t key, bool mode)
{
	if (key == MLX_KEY_C)
	{
		mode = true;
		if (data->god)
			data->god = false;
		data->render_sel = render_fast;
	}
	else
		set_last(data);
	return (mode);
}

void	key_cam_move(t_data *data, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_DOWN && keydata.modifier == MLX_CONTROL)
		data->cam->fov = fmax(data->cam->fov - 1, 0);
	else if (keydata.key == MLX_KEY_UP && keydata.modifier == MLX_CONTROL)
		data->cam->fov = fmin(180, data->cam->fov + 1);
	else if (keydata.key == MLX_KEY_UP)
		data->cam->pos.y += CAMPLUS;
	else if (keydata.key == MLX_KEY_DOWN)
		data->cam->pos.y -= CAMPLUS;
	else if (keydata.key == MLX_KEY_RIGHT)
		data->cam->pos.x += CAMPLUS;
	else if (keydata.key == MLX_KEY_LEFT)
		data->cam->pos.x -= CAMPLUS;
	set_last(data);
}

void	press_keyhook(t_data *data, mlx_key_data_t keydata)
{
	static bool	mode;

	if (keydata.key == MLX_KEY_SPACE)
	{
		if (data->last_render != FAST)
			mode = !mode;
		if (data->img_last)
			free_image_all(data, data->img_last);
		data->img_last = NULL;
		free(data->accum_r);
		free(data->accum_g);
		free(data->accum_b);
		data->accum_r = NULL;
		data->accum_g = NULL;
		data->accum_b = NULL;
		data->sample_count = 0;
		if (!mode)
		{
			if (data->god)
				data->god = false;
			data->render_sel = render_one;
		}
		else
			data->render_sel = update_render;
	}
	else if (keydata.key == MLX_KEY_C)
		mode = press_keyhook_normi(data, keydata.key, mode);
}

void	my_keyhook(mlx_key_data_t keydata, void *param)
{
	t_data		*data;

	data = param;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		mlx_terminate(data->mlx);
		exit(EXIT_SUCCESS);
	}
	else if (keydata.action == MLX_PRESS)
		press_keyhook(data, keydata);
	else if (data->last_render == FAST)
		key_cam_move(data, keydata);
}

void	resise_w(int32_t width, int32_t height, void *param)
{
	t_data	*data;

	(void)width;
	(void)height;
	data = param;
	data->god = false;
	data->img_last = NULL;
	data->sample_count = 0;
	free(data->accum_r);
	free(data->accum_g);
	free(data->accum_b);
	data->accum_r = NULL;
	data->accum_g = NULL;
	data->accum_b = NULL;
	if (data->last_render == ONE)
		data->render_sel = render_one;
	else if (data->last_render == FAST)
		data->render_sel = render_fast;
	else if (data->last_render == UPDATE)
		data->render_sel = update_render;
}
