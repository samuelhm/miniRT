/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:37:51 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 12:04:00 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "render.h"
#include "threads.h"

void	*process_rows(void *arg)
{
	t_thread_data	*td;
	int				thread_id;
	int				y;
	int				x;

	td = (t_thread_data *)arg;
	thread_id = td->thread_id;
	y = thread_id;
	while (y < td->data->y)
	{
		x = 0;
		while (x < td->data->x)
		{
			pthread_mutex_lock(td->data->m_god);
			if (!td->data->god)
			{
				pthread_mutex_unlock(td->data->m_god);
				pthread_exit(NULL);
				return (NULL);
			}
			pthread_mutex_unlock(td->data->m_god);
			td->image[y][x] = trace_ray(td->rays[y][x], td->data);
			x++;
		}
		y += NUM_THREADS;
	}
	pthread_exit(NULL);
}

void	render_with_threads(t_data *data, t_ray **rays, uint32_t **image)
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
		pthread_create(&threads[i], NULL, process_rows, &thread_data[i]);
		i++;
	}
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

uint32_t	**average_samples(t_data *data, uint32_t **s1, uint32_t **s2)
{
	uint32_t	**res;
	int			x;
	int			y;

	x = -1;
	res = init_image_(data);
	while (++x < data->y)
	{
		y = -1;
		while (++y < data->x)
			res[x][y] = average(s1[x][y], s2[x][y]);
	}
	return (res);
}

uint32_t	**render(t_data *data, int mode)
{
	t_ray		**rays;
	t_vp		*vp;
	uint32_t	**image;

	vp = init_viewport(data->cam, data->x, data->y);
	if (!mode)
		rays = init_rays(data, data->cam, vp);
	else
		rays = init_raysc(data, data->cam, vp);
	image = init_image_(data);
	if (!image)
		return (NULL);
	render_with_threads(data, rays, image);
	free_render(data, vp, rays);
	return (image);
}
