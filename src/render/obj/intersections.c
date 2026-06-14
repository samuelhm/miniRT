/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:48:44 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/16 10:47:27 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"
#include "libvct.h"

bool	calc_quad_sphere(t_obj *sphere, t_ray ray, t_quadratic *quad)
{
	t_v3	oc;

	oc = vsub(ray.origin, sphere->pos);
	quad->a = dot(ray.direction, ray.direction);
	quad->b = 2.0f * dot(ray.direction, oc);
	quad->c = dot(oc, oc) - (sphere->calcs.radius2);
	init_quadratic(quad, quad->a, quad->b, quad->c);
	if (!solve_quadratic(quad))
		return (false);
	return (true);
}

bool	hit_sp(t_ray *ray, t_obj *sphere, double *t)
{
	t_quadratic	quad;

	if (!calc_quad_sphere(sphere, *ray, &quad))
		return (false);
	if (quad.t1 > EPSILON && quad.t1 < *t)
		*t = quad.t1;
	else
		return (false);
	ray->point = vadd(ray->origin, vmul(*t, ray->direction));
	ray->normal = normalize(vsub(ray->point, sphere->pos));
	if (sphere->material.bm_texture)
		get_sphere_normal(sphere, ray);
	return (true);
}

static bool	side_mode(t_obj *plane, t_v3 local_hit)
{
	double	x;
	double	y;

	x = dot(local_hit, plane->right);
	y = dot(local_hit, plane->up);
	if (plane->face == 1 && (fabs(x) > plane->calcs.half_size.y || \
		fabs(y) > plane->calcs.half_size.z))
		return (false);
	else if (plane->face == 2 && (fabs(x) > plane->calcs.half_size.x \
				|| fabs(y) > plane->calcs.half_size.z))
		return (false);
	else if (plane->face == 3 && (fabs(x) > plane->calcs.half_size.x \
				|| fabs(y) > plane->calcs.half_size.y))
		return (false);
	return (true);
}

bool	hit_pl(t_data *data, t_ray *ray, t_obj *plane, double *t)
{
	double	denom;
	double	numer;
	double	result;
	t_v3	hit_point;
	t_v3	local_point;

	denom = dot(ray->direction, plane->axis);
	if (fabs(denom) < EPSILON)
		return (false);
	numer = plane->calcs.numerator;
	if (!v3_compare(data->cam->pos, ray->origin))
		numer = dot(vsub(plane->pos, ray->origin), plane->axis);
	result = numer / denom;
	if (result <= EPSILON || result >= *t)
		return (false);
	hit_point = vadd(ray->origin, vmul(result, ray->direction));
	local_point = vsub(hit_point, plane->pos);
	if (plane->type == SIDE && !side_mode(plane, local_point))
		return (false);
	ray->point = hit_point;
	*t = result;
	ray->normal = plane->axis;
	if (denom > EPSILON)
		ray->normal = plane->calcs.i_axis;
	if (plane->material.bm_texture)
		get_plane_normal(plane, ray->point, ray);
	return (true);
}
