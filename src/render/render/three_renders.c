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
	uint32_t	*new_img;

	data = (t_data *)param;
	data->x = data->mlx->width;
	data->y = data->mlx->height;
	data->god = true;
	data->sample_mode = 1;
	mlx_resize_image(data->img, (uint32_t)data->x, (uint32_t)data->y);
	if (!data->img->enabled)
		data->img->enabled = true;
	new_img = render(data, 1);
	fill_image(data, (uint32_t *)data->img->pixels, new_img);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	free(data->img_last);
	data->img_last = new_img;
	data->render_sel = NULL;
	data->last_render = ONE;
}

void	render_fast(void *param)
{
	t_data		*data;
	uint32_t	*new_img;

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

static void	accum_init(t_data *data)
{
	int	total;

	total = data->y * data->x;
	if (!data->accum_r)
	{
		data->accum_r = calloc(total, sizeof(float));
		data->accum_g = calloc(total, sizeof(float));
		data->accum_b = calloc(total, sizeof(float));
	}
}

static void	accum_add(t_data *data, uint32_t *new_img)
{
	int		i;
	int		total;
	t_rgb	c;

	total = data->y * data->x;
	i = -1;
	while (++i < total)
	{
		unpack(new_img[i], &c);
		data->accum_r[i] += (float)c.r;
		data->accum_g[i] += (float)c.g;
		data->accum_b[i] += (float)c.b;
	}
}

static void	accum_display(t_data *data)
{
	int			i;
	int			total;
	uint32_t	*pixels;
	float		inv_n;
	t_rgb		c;

	total = data->y * data->x;
	pixels = (uint32_t *)data->img->pixels;
	inv_n = 1.0f / (float)data->sample_count;
	i = -1;
	while (++i < total)
	{
		c.r = (unsigned char)(data->accum_r[i] * inv_n);
		c.g = (unsigned char)(data->accum_g[i] * inv_n);
		c.b = (unsigned char)(data->accum_b[i] * inv_n);
		pixels[i] = get_colour(c);
	}
}

void	update_render(void *param)
{
	t_data		*data;
	uint32_t	*new_img;
	t_ll		time;

	time = current_timestamp();
	data = (t_data *)param;
	data->x = data->mlx->width;
	data->y = data->mlx->height;
	data->god = true;
	data->sample_mode = 0;
	mlx_resize_image(data->img, (uint32_t)data->x, (uint32_t)data->y);
	accum_init(data);
	if (!data->img->enabled)
		data->img->enabled = true;
	new_img = render(data, 0);
	data->sample_count++;
	accum_add(data, new_img);
	free_image_all(data, new_img);
	accum_display(data);
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	time = current_timestamp() - time;
	data->last_render = UPDATE;
}
