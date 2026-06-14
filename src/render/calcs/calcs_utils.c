/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcs_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 17:48:01 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_v3	random_in_hemisphere(t_v3 normal)
{
	t_v3		random_vector;
	double		u1;
	double		u2;
	double		theta;
	double		phi;

	u1 = (double)rand() / RAND_MAX;
	u2 = (double)rand() / RAND_MAX;
	theta = acos(sqrt(1 - u1));
	phi = 2 * M_PI * u2;
	random_vector.x = sin(theta) * cos(phi);
	random_vector.y = sin(theta) * sin(phi);
	random_vector.z = cos(theta);
	if (dot(random_vector, normal) < 0)
		random_vector = vmul(-1.0f, random_vector);
	return (normalize(random_vector));
}

t_rgb	apply_self_emission(t_obj *obj, t_rgb base_color)
{
	if (obj->material.m_type == EM)
		return (color_add(base_color, color_mul(obj->rgb, \
							obj->material.self_emision)));
	return (base_color);
}

void	compute_emissive_light(t_obj *emitter, t_ray *ray, \
								t_rgb *color, t_data *data)
{
	t_v3	light_direction;
	t_ray	shadow_ray;
	t_rgb	emitted_color;
	double	intensity;

	light_direction = normalize(vsub(emitter->pos, ray->point));
	shadow_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	shadow_ray.direction = light_direction;
	if (!data_shadow(data, &shadow_ray, \
		vlength(vsub(emitter->pos, ray->point)), emitter))
	{
		intensity = fmax(0.0f, dot(light_direction, ray->normal));
		emitted_color = color_mul(emitter->rgb, emitter->material.emision);
		*color = color_add(*color, color_mul(emitted_color, intensity));
	}
}

void	iter_lights(t_data *data, t_obj *obj, t_ray *ray, t_direct d)
{
	while (d.slight)
	{
		d.vsub_pos_point = vsub(d.slight->pos, ray->point);
		d.shadow_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
		d.shadow_ray.direction = normalize(d.vsub_pos_point);
		if (data_shadow(data, &d.shadow_ray, vlength(d.vsub_pos_point), NULL))
		{
			d.slight = d.slight->next;
			continue ;
		}
		d.intensity = fmax(0.0f, dot(d.shadow_ray.direction, ray->normal));
		difuse_light(d.color, d.slight, obj, d.intensity);
		if (obj->material.specularity > 0)
		{
			d.reflect_dir = reflect(vmul(-1, d.shadow_ray.direction), \
										ray->normal);
			d.view_dir = normalize(vmul(-1, ray->direction));
			d.spec_intensity = pow(fmax(dot(d.reflect_dir, d.view_dir), 0.00), \
			obj->material.shininess);
			d.specular_color = color_add(d.specular_color, \
									color_mul(d.slight->rgb, d.spec_intensity \
											* obj->material.specularity));
		}
		d.slight = d.slight->next;
	}
}

bool	pt_checks(t_obj *closest, t_ray *ray, t_rgb *dirb)
{
	if (!closest)
		return (true);
	if (closest->type == PL && \
		closest->material.board_scale != -1)
		dirb[3] = checkerboard_color(closest, ray->point);
	else
		dirb[3] = closest->a_rgb;
	return (false);
}
