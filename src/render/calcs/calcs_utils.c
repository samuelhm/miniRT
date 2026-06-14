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

#include "rt.h"

_Thread_local static uint32_t	tls_rng_state;
_Thread_local static bool		tls_rng_seeded;

void	tls_rng_seed(uint32_t seed)
{
	tls_rng_state = seed ? seed : 1;
	tls_rng_seeded = true;
}

static uint32_t	tls_xorshift32(void)
{
	uint32_t	x;

	x = tls_rng_state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	tls_rng_state = x;
	return (x);
}

double	tls_random_double(void)
{
	return ((double)tls_xorshift32()) / 4294967296.0;
}

t_v3	random_in_hemisphere(t_v3 normal)
{
	t_v3		local;
	t_v3		tangent;
	t_v3		bitangent;
	double		u1;
	double		u2;
	double		theta;
	double		phi;

	u1 = tls_random_double();
	u2 = tls_random_double();
	theta = acos(sqrt(u1));
	phi = 2.0 * M_PI * u2;
	local.x = sin(theta) * cos(phi);
	local.y = sin(theta) * sin(phi);
	local.z = cos(theta);
	if (fabs(normal.x) > 0.9)
		tangent = normalize(cross((t_v3){0.0, 1.0, 0.0}, normal));
	else
		tangent = normalize(cross((t_v3){1.0, 0.0, 0.0}, normal));
	bitangent = cross(normal, tangent);
	local = vadd(vadd(vmul(local.x, tangent), vmul(local.y, bitangent)),
			vmul(local.z, normal));
	return (normalize(local));
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

bool	pt_checks(t_obj *closest, t_ray *ray, t_rgb *base_color)
{
	if (!closest)
		return (true);
	if (closest->type == PL && closest->material.board_scale != -1)
		*base_color = checkerboard_color(closest, ray->point);
	else
		*base_color = closest->a_rgb;
	return (false);
}
