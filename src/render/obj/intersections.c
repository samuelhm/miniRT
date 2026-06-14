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

#include "miniRT.h"
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

bool	side_mode(t_obj *plane, t_v3 local_hit)
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
	double	dnrxy[3];
	t_v3	hits_lp[2];

	dnrxy[0] = dot(ray->direction, plane->axis);
	if (fabs(dnrxy[0]) < EPSILON)
		return (false);
	dnrxy[1] = plane->calcs.numerator;
	if (!v3_compare(data->cam->pos, ray->origin))
		dnrxy[1] = dot(vsub(plane->pos, ray->origin), plane->axis);
	dnrxy[2] = dnrxy[1] / dnrxy[0];
	if (dnrxy[2] <= EPSILON || dnrxy[2] >= *t)
		return (false);
	hits_lp[1] = vadd(ray->origin, vmul(dnrxy[2], ray->direction));
	hits_lp[0] = vsub(hits_lp[1], plane->pos);
	if (plane->type == SIDE)
		if (!side_mode(plane, hits_lp[0]))
			return (false);
	ray->point = hits_lp[1];
	*t = dnrxy[2];
	ray->normal = plane->axis;
	if (dnrxy[0] > EPSILON)
		ray->normal = plane->calcs.i_axis;
	if (plane->material.bm_texture)
		get_plane_normal(plane, ray->point, ray);
	return (true);
}
