/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:00:21 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 13:20:57 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"
#include "libvct.h"

void	set_cy_axis(t_quadratic *quad, t_obj *cy, t_ray *ray)
{
	t_v3	oc_perp;
	t_v3	d_perp;
	double	oc_axis_dot;
	double	d_axis_dot;
	t_v3	oc;

	oc = vsub(ray->origin, cy->pos);
	oc_axis_dot = dot(oc, cy->axis);
	d_axis_dot = dot(ray->direction, cy->axis);
	oc_perp = vsub(oc, vmul(oc_axis_dot, cy->axis));
	d_perp = vsub(ray->direction, vmul(d_axis_dot, cy->axis));
	quad->c = dot(oc_perp, oc_perp) - cy->calcs.radius2;
	quad->a = dot(d_perp, d_perp);
	quad->b = 2.0 * dot(oc_perp, d_perp);
}

static double	compute_intersection(t_ray *ray, t_quadratic quad, t_v3 *point)
{
	double	t_min;

	t_min = INFINITY;
	if (quad.t1 > EPSILON && quad.t1 < quad.t2)
	{
		*point = vadd(ray->origin, vmul(quad.t1, ray->direction));
		t_min = quad.t1;
	}
	else if (quad.t2 > EPSILON)
	{
		*point = vadd(ray->origin, vmul(quad.t2, ray->direction));
		t_min = quad.t2;
	}
	return (t_min);
}

double	set_ray_t(t_ray *ray, t_obj *cy, double *t, t_quadratic quad)
{
	double	t_min;
	double	proj;
	t_v3	point;

	t_min = compute_intersection(ray, quad, &point);
	proj = dot(vsub(point, cy->pos), cy->axis);
	if (proj > -cy->calcs.hh_e_sum && proj < cy->calcs.hh_e_res && t_min < *t)
	{
		*t = t_min;
		ray->point = point;
		ray->normal = normalize(vsub(point, \
			vadd(cy->pos, vmul(proj, cy->axis))));
		if (fabs(t_min - quad.t2) < EPSILON && dot(ray->direction, ray->normal) > 0)
			ray->normal = vmul(-1, ray->normal);
	}
	return (t_min);
}

bool	hit_cy(t_ray *ray, t_obj *cy, double *t)
{
	t_quadratic	quad;
	double		t_min;

	t_min = INFINITY;
	set_cy_axis(&quad, cy, ray);
	init_quadratic(&quad, quad.a, quad.b, quad.c);
	if (solve_quadratic2(&quad))
	{
		if (quad.t1 <= EPSILON && quad.t2 <= EPSILON)
			return (false);
		t_min = set_ray_t(ray, cy, t, quad);
	}
	return (t_min < INFINITY);
}
