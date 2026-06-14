/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 01:56:28 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 12:04:09 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "threads.h"

uint32_t	trace_fast(t_ray ray, t_data *data)
{
	double	t_min;
	t_obj	*closest_obj;
	t_rgb	alight;

	alight.r = 50;
	alight.g = 50;
	alight.b = 50;
	t_min = INFINITY;
	closest_obj = find_closest(data, &ray, &t_min);
	if (!closest_obj)
		return (BLACK);
	return (get_colour(color_add(alight, closest_obj->a_rgb)));
}

static void	fill_block(t_thread_data *td, int y, int x, uint32_t color)
{
	int	i;
	int	j;

	i = 0;
	while (i < 10)
	{
		j = 0;
		while (j < 10)
		{
			if ((y + i) < td->data->y && (x + j) < td->data->x)
				td->image[(y + i) * td->data->x + (x + j)] = color;
			j++;
		}
		i++;
	}
}

void	*cprocess_rows(void *arg)
{
	t_thread_data	*td;
	int				y;
	int				x;
	uint32_t		color;

	td = (t_thread_data *)arg;
	y = td->thread_id;
	while (y < td->data->y)
	{
		x = 0;
		while (x < td->data->x)
		{
			if ((y % 10) == 0 && (x % 10) == 0)
			{
				color = trace_fast(td->rays[y][x], td->data);
				fill_block(td, y, x, color);
			}
			x++;
		}
		y += NUM_THREADS;
	}
	pthread_exit(NULL);
}

void	c_render(t_data *data, t_ray **rays, uint32_t *image)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				i;

	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].thread_id = i;
		thread_data[i].rays = rays;
		thread_data[i].data = data;
		thread_data[i].image = image;
		pthread_create(&threads[i], NULL, cprocess_rows, &thread_data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

uint32_t	*console_render(t_data *data)
{
	t_ray		**rays;
	t_vp		*vp;
	uint32_t	*image;

	vp = init_viewport(data->cam, data->x, data->y);
	rays = init_raysc(data, data->cam, vp);
	if (!rays)
	{
		free(vp);
		return (NULL);
	}
	image = init_image_(data);
	if (!image)
	{
		free_render(data, vp, rays);
		return (NULL);
	}
	c_render(data, rays, image);
	free_render(data, vp, rays);
	return (image);
}
