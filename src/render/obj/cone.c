/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 16:09:01 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/13 11:48:59 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	cone_calc(t_ray *ray, t_obj *cone, t_v3 *oc, t_quadratic *quad)
{
	double	dot_dir_axis;
	double	dot_oc_axis;

	*oc = vsub(ray->origin, cone->pos);
	dot_oc_axis = dot(*oc, cone->axis);
	dot_dir_axis = dot(ray->direction, cone->axis);
	quad->a = dot_dir_axis * dot_dir_axis - cone->calcs.k * \
				dot(ray->direction, ray->direction);
	quad->b = 2.0f * (dot_dir_axis * dot_oc_axis - \
				cone->calcs.k * dot(ray->direction, *oc));
	quad->c = dot_oc_axis * dot_oc_axis - cone->calcs.k * dot(*oc, *oc);
	init_quadratic(quad, quad->a, quad->b, quad->c);
}

static bool	cone_hit(t_ray *ray, t_obj *cone, double t_min, double *t)
{
	t_v3	point;
	t_v3	cone_to_point;
	double	height_check;
	double	m;

	point = vadd(ray->origin, vmul(t_min, ray->direction));
	height_check = dot(vsub(point, cone->pos), cone->axis);
	if (height_check < 0.0f || height_check > cone->height)
		return (false);
	if (*t > t_min)
	{
		*t = t_min;
		ray->point = point;
		cone_to_point = vsub(point, cone->pos);
		m = dot(cone_to_point, cone->axis);
		ray->normal = normalize(vsub(cone_to_point, vmul(m, cone->axis)));
		if (dot(ray->direction, ray->normal) > 0)
			ray->normal = vmul(-1.0f, ray->normal);
	}
	return (true);
}

bool	hit_cone(t_ray *ray, t_obj *cone, double *t)
{
	t_v3		oc;
	t_quadratic	quad;

	cone_calc(ray, cone, &oc, &quad);
	if (!solve_quadratic(&quad))
		return (false);
	if (cone_hit(ray, cone, quad.t1, t))
		return (true);
	return (cone_hit(ray, cone, quad.t2, t));
}
