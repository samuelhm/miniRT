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

static void	trace_fast_light(t_rgb *result, t_obj *obj, t_ray *ray,
		t_data *data)
{
	t_slight	*slight;
	t_v3		light_dir;
	double		intensity;

	slight = data->s_light;
	while (slight)
	{
		light_dir = normalize(vsub(slight->pos, ray->point));
		intensity = fmax(0.0, dot(light_dir, ray->normal));
		if (intensity > 0.0)
			difuse_light(result, slight, obj, intensity);
		slight = slight->next;
	}
}

uint32_t	trace_fast(t_ray ray, t_data *data)
{
	double	t_min;
	t_obj	*closest_obj;
	t_rgb	result;

	result.r = 50;
	result.g = 50;
	result.b = 50;
	t_min = INFINITY;
	closest_obj = find_closest(data, &ray, &t_min);
	if (!closest_obj)
		return (BLACK);
	result = color_add(result, closest_obj->a_rgb);
	if (closest_obj->material.m_type == EM)
		result = apply_self_emission(closest_obj, result);
	trace_fast_light(&result, closest_obj, &ray, data);
	return (get_colour(result));
}

static void	fill_block(t_thread_data *td, int y, int x, uint32_t color)
{
	int	i;
	int	j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
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
	t_ray			ray;
	uint32_t		color;

	td = (t_thread_data *)arg;
	y = td->thread_id;
	while (y < td->data->y)
	{
		x = 0;
		while (x < td->data->x)
		{
			if ((y % 4) == 0 && (x % 4) == 0)
			{
				ray = generate_ray(td->data, td->vp, td->cam, x, y, 1);
				color = trace_fast(ray, td->data);
				fill_block(td, y, x, color);
			}
			x++;
		}
		y += NUM_THREADS;
	}
	pthread_exit(NULL);
}

void	c_render(t_data *data, t_vp *vp, uint32_t *image)
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
		thread_data[i].mode = 1;
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
	t_vp		*vp;
	uint32_t	*image;

	vp = init_viewport(data->cam, data->x, data->y);
	image = init_image_(data);
	if (!image)
	{
		free(vp);
		return (NULL);
	}
	c_render(data, vp, image);
	free(vp);
	return (image);
}
