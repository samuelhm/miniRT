/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 11:57:06 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

_Thread_local int	g_dbg = 0;
_Thread_local int	g_dbg_depth = 0;

static void	dbg_indent(void)
{
	for (int i = 0; i < g_dbg_depth; i++)
		fprintf(stderr, "  ");
}

t_obj	*find_closest(t_data *restrict data, t_ray *restrict ray,
		double *restrict t_min)
{
	t_obj	*closest_obj;
	t_obj	*obj;
	double	t;
	double	bvh_t_min;

	if (data->bvh_nodes)
	{
		bvh_t_min = *t_min;
		closest_obj = bvh_traverse(data, ray, &bvh_t_min);
		if (g_dbg)
		{
			dbg_indent();
			if (closest_obj)
				fprintf(stderr, "BVH hit type=%d t=%.2f pos=(%.2f,%.2f,%.2f)\n",
					closest_obj->type, bvh_t_min,
					closest_obj->pos.x, closest_obj->pos.y,
					closest_obj->pos.z);
			else
				fprintf(stderr, "BVH MISS\n");
		}
		if (closest_obj)
		{
			*t_min = bvh_t_min;
			return (closest_obj);
		}
	}
	closest_obj = NULL;
	obj = data->obj;
	while (obj)
	{
		t = *t_min;
		if ((obj->type == SP && hit_sp(ray, obj, &t)) \
		|| ((obj->type == PL || obj->type == SIDE) && \
							hit_pl(data, ray, obj, &t)) \
		|| (obj->type == CY && hit_cy(ray, obj, &t)) \
		|| (obj->type == CAP && hit_cap(data, ray, obj, &t)) \
		|| (obj->type == CO && hit_cone(ray, obj, &t)))
		{
			if (t > 0 && t < *t_min)
			{
				*t_min = t;
				closest_obj = obj;
			}
		}
		obj = obj->next;
	}
	if (g_dbg)
	{
		dbg_indent();
		if (closest_obj)
			fprintf(stderr, "FALLBACK hit type=%d t=%.2f rgb=(%d,%d,%d)\n",
				closest_obj->type, *t_min,
				closest_obj->rgb.r, closest_obj->rgb.g,
				closest_obj->rgb.b);
		else
			fprintf(stderr, "FALLBACK MISS\n");
	}
	if (data->bvh_nodes && closest_obj && !g_dbg)
	{
		static _Thread_local int	bvh_miss;
		if (bvh_miss < 5)
		{
			bvh_miss++;
			fprintf(stderr, "BVH_MISS o(%.2f,%.2f,%.2f) d(%.4f,%.4f,%.4f)"
				" hit=%d\n", ray->origin.x, ray->origin.y,
				ray->origin.z, ray->direction.x, ray->direction.y,
				ray->direction.z, closest_obj->type);
		}
	}
	return (closest_obj);
}

t_rgb	diffuse_ray(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	new_ray;
	t_rgb	trace_color;

	new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	new_ray.direction = random_in_hemisphere(ray->normal);
	trace_color = path_trace(&new_ray, data, depth - 1);
	return (color_mul(trace_color, closest->material.reflectivity));
}

t_rgb	compute_direct_light(t_obj *obj, t_data *data, \
							t_ray *ray, t_rgb color)
{
	t_obj		*current_obj;
	t_direct	d;

	d.color = &color;
	current_obj = data->obj;
	d.specular_color = rgbdefine(0, 0, 0);
	d.slight = data->s_light;
	if (current_obj)
	{
		while (current_obj)
		{
			if (current_obj->material.m_type == EM && current_obj != obj)
				compute_emissive_light(current_obj, ray, &color, data);
			current_obj = current_obj->next;
		}
	}
	iter_lights(data, obj, ray, d);
	color = color_add(color, d.specular_color);
	return (color);
}

t_rgb	path_trace(t_ray *ray, t_data *data, int depth)
{
	t_obj	*closest;
	t_rgb	base_color;
	t_rgb	direct_light;
	t_rgb	indirect_light;
	t_rgb	result;
	double	t;
	t_v2	uv;

	t = INFINITY;
	closest = find_closest(data, ray, &t);
	if (closest && closest->material.texture)
	{
		uv = calculate_uv(ray->point, closest);
		return (texture_color(closest, uv));
	}
	if (pt_checks(closest, ray, &base_color))
		return (rgbdefine(0, 0, 0));
	if (depth <= 0)
		return (rgbdefine(0, 0, 0));
	direct_light = compute_direct_light(closest, data, ray, rgbdefine(0, 0, 0));
	if (closest->material.m_type == EM)
		base_color = apply_self_emission(closest, base_color);
	if (data->sample_mode && (closest->material.m_type == MAT_DEFAULT
			|| closest->material.m_type == MT))
		indirect_light = rgbdefine(0, 0, 0);
	else
		indirect_light = path_trace_type(ray, closest, data, depth);
	result = color_add(color_add(base_color, direct_light), indirect_light);
	return (result);
}

uint32_t	trace_ray(t_ray ray, t_data *data)
{
	double		t_min;
	t_obj		*closest_obj;
	t_rgb		c_global;

	t_min = INFINITY;
	closest_obj = find_closest(data, &ray, &t_min);
	if (!closest_obj)
	{
		static _Thread_local int	miss_n;
		if (miss_n < 10)
		{
			miss_n++;
			fprintf(stderr, "MISS o(%.2f,%.2f,%.2f) d(%.6f,%.6f,%.6f)\n",
				ray.origin.x, ray.origin.y, ray.origin.z,
				ray.direction.x, ray.direction.y, ray.direction.z);
		}
		return (BLACK);
	}
	c_global = path_trace(&ray, data, MAX_DEPTH);
	if (closest_obj->material.texture)
	{
		t_v2	uv = calculate_uv(ray.point, closest_obj);
		if (uv.u < 0 || uv.v < 0)
			return (get_colour(c_global));
		if (closest_obj->material.bm_texture)
			return (texture_weight(c_global, texture_color(closest_obj, uv)));
		return (get_colour(texture_color(closest_obj, uv)));
	}
	return (get_colour(c_global));
}

uint32_t	trace_ray_dbg(t_ray ray, t_data *data, int px, int py)
{
	(void)px;
	(void)py;
	return (trace_ray(ray, data));
}
