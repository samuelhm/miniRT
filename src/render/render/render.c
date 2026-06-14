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

#include "rt.h"
#include "render.h"
#include "threads.h"

t_ray	generate_ray(t_data *data, t_vp *vp, t_cam *cam, int x, int y,
		int mode)
{
	t_ray	ray;
	double	uv[2];

	if (!mode)
	{
		uv[0] = ((double)x + tls_random_double()) / (double)(data->x - 1);
		uv[1] = 1.0f - ((double)y + tls_random_double()) / (double)(data->y - 1);
	}
	else
	{
		uv[0] = (double)x / (double)(data->x - 1);
		uv[1] = 1.0f - (double)y / (double)(data->y - 1);
	}
	ray.origin = cam->pos;
	ray.direction = normalize(vsub(vadd(vadd(vp->lower_left,
					vmul(uv[0], vp->horizontal)),
					vmul(uv[1], vp->vertical)), cam->pos));
	ray.i_direction = normalize(vneg(ray.direction));
	return (ray);
}

void	*process_rows(void *arg)
{
	t_thread_data	*td;
	int				y;
	int				x;

	td = (t_thread_data *)arg;
	tls_rng_seed((uint32_t)((uintptr_t)pthread_self()
			^ (td->thread_id * 2654435761U)));
	y = td->thread_id;
	while (y < td->data->y)
	{
		x = 0;
		while (x < td->data->x)
		{
			if (!td->data->god)
			{
				pthread_exit(NULL);
				return (NULL);
			}
			t_ray	ray = generate_ray(td->data, td->vp, td->cam, x, y,
					td->mode);
			td->image[y * td->data->x + x] = trace_ray(ray, td->data);
			x++;
		}
		y += NUM_THREADS;
	}
	pthread_exit(NULL);
}

void	render_with_threads(t_data *data, t_vp *vp, uint32_t *image, int mode)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				i;

	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].thread_id = i;
		thread_data[i].vp = vp;
		thread_data[i].cam = data->cam;
		thread_data[i].mode = mode;
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

uint32_t	*render(t_data *data, int mode)
{
	t_vp		*vp;
	uint32_t	*image;

	vp = init_viewport(data->cam, data->x, data->y);
	if (!vp)
		return (NULL);
	image = init_image_(data);
	if (!image)
	{
		free(vp);
		return (NULL);
	}
	render_with_threads(data, vp, image, mode);
	free(vp);
	return (image);
}
