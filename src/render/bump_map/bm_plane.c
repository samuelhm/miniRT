/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bm_plane.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:27:29 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/16 10:44:28 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	calc_pl_tb(t_v3 normal, t_v3 *tangent, t_v3 *bitangent)
{
	t_v3	up;

	if (fabs(normal.y) < 0.99)
		up = vdefine(0, 1, 0);
	else
		up = vdefine(1, 0, 0);
	*tangent = cross(up, normal);
	*tangent = normalize(*tangent);
	*bitangent = cross(normal, *tangent);
	*bitangent = normalize(*bitangent);
}

static t_v3	transform_tws(t_v3 tang, t_v3 bitang, t_v3 normal, t_v3 mp_nrmal)
{
	t_v3	w_normal;

	w_normal.x = mp_nrmal.x * tang.x + mp_nrmal.y \
				* bitang.x + mp_nrmal.z * normal.x;
	w_normal.y = mp_nrmal.x * tang.y + mp_nrmal.y \
				* bitang.y + mp_nrmal.z * normal.y;
	w_normal.z = mp_nrmal.x * tang.z + mp_nrmal.y \
				* bitang.z + mp_nrmal.z * normal.z;
	return (normalize(w_normal));
}

static t_v3	get_normal_from_map_plane(t_obj *plane, t_v3 hp)
{
	uint32_t	index;
	t_rgb		rgb;
	t_v3		normal;
	double		uv[2];
	int			x;
	int			y;

	uv[0] = fmod(hp.x, 1.0);
	if (uv[0] < 0)
		uv[0] += 1.0;
	uv[1] = fmod(hp.z, 1.0);
	if (uv[1] < 0)
		uv[1] += 1.0;
	x = (int)(uv[0] * plane->material.bm_texture->width \
			* plane->material.bm_size) % (int)plane->material.bm_texture->width;
	y = (int)(uv[1] * plane->material.bm_texture->height \
			* plane->material.bm_size) % (int)plane->material.bm_texture->height;
	index = (uint32_t)(y * (int)plane->material.bm_texture->width + x) \
			* plane->material.bm_texture->bytes_per_pixel;
	rgb.r = plane->material.bm_texture->pixels[index];
	rgb.g = plane->material.bm_texture->pixels[index + 1];
	rgb.b = plane->material.bm_texture->pixels[index + 2];
	normal.x = (rgb.r / 255.0) * 2.0 - 1.0;
	normal.y = (rgb.g / 255.0) * 2.0 - 1.0;
	normal.z = (rgb.b / 255.0) * 2.0 - 1.0;
	return (normalize(normal));
}

void	get_plane_normal(t_obj *plane, t_v3 hit_point, t_ray *ray)
{
	t_v3	map_normal;
	t_v3	tang;
	t_v3	bit;

	if (plane->material.bm_texture)
	{
		map_normal = get_normal_from_map_plane(plane, hit_point);
		calc_pl_tb(plane->axis, &tang, &bit);
		ray->normal = transform_tws(tang, bit, plane->axis, map_normal);
	}
}
