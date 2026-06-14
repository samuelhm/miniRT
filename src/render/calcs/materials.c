/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 17:32:27 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_rgb	glass_ray(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	reflected_ray;
	t_ray	refracted_ray;
	t_rgb	reflected_color;
	t_rgb	refracted_color;

	closest->calcs.kr = fresnel(closest, ray->direction, ray->normal);
	refracted_ray.origin = vadd(ray->point, vmul(-EPSILON, ray->normal));
	refracted_ray.direction = refract(closest, ray->direction, ray->normal);
	reflected_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	reflected_ray.direction = reflect(ray->direction, ray->normal);
	reflected_color = path_trace(&reflected_ray, data, depth - 1);
	refracted_color = path_trace(&refracted_ray, data, depth - 1);
	return (color_add(color_mul(reflected_color, closest->calcs.kr), \
					color_mul(refracted_color, 1.0f - closest->calcs.kr)));
}

t_rgb	mirror_ray(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray		new_ray;
	t_v3		normal;
	t_rgb		pt;
	t_rgb		res;

	normal = ray->normal;
	new_ray.origin = vadd(ray->point, vmul(EPSILON, normal));
	new_ray.direction = reflect(ray->direction, normal);
	pt = path_trace(&new_ray, data, depth - 1);
	res = color_mul(pt, closest->material.reflectivity);
	return (res);
}

t_v3	perturb_vector(t_v3 direction, double roughness, t_v3 normal)
{
	t_v3	random_vector;

	random_vector = random_in_hemisphere(normal);
	return (normalize(vadd(direction, vmul(roughness, random_vector))));
}

t_rgb	metallic_ray(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray		new_ray;
	t_v3		perturbed_direction;

	new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	new_ray.direction = reflect(ray->direction, ray->normal);
	if (closest->material.roughness > 0)
	{
		perturbed_direction = perturb_vector(new_ray.direction, \
					closest->material.roughness, ray->normal);
		new_ray.direction = perturbed_direction;
	}
	return (color_mul(path_trace(&new_ray, data, depth - 1), \
							closest->material.reflectivity));
}
