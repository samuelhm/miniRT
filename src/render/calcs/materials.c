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
	double	dot_ndir;
	int		exiting;

	closest->calcs.kr = fresnel(closest, ray->direction, ray->normal);
	dot_ndir = dot(ray->direction, ray->normal);
	exiting = (dot_ndir > 0);
	if (exiting)
	{
		refracted_ray.origin = vadd(ray->point,
				vmul(EPSILON, ray->normal));
		reflected_ray.origin = vadd(ray->point,
				vmul(-EPSILON, ray->normal));
	}
	else
	{
		refracted_ray.origin = vadd(ray->point,
				vmul(-EPSILON, ray->normal));
		reflected_ray.origin = vadd(ray->point,
				vmul(EPSILON, ray->normal));
	}
	refracted_ray.direction = refract(closest, ray->direction, ray->normal);
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

static void	build_tangent_frame(t_v3 normal, t_v3 *tangent, t_v3 *bitangent)
{
	if (fabs(normal.x) > 0.9)
		*tangent = normalize(cross((t_v3){0.0, 1.0, 0.0}, normal));
	else
		*tangent = normalize(cross((t_v3){1.0, 0.0, 0.0}, normal));
	*bitangent = cross(normal, *tangent);
}

static t_v3	ggx_sample(t_v3 normal, double roughness)
{
	double	u1;
	double	u2;
	double	a;
	double	phi;
	double	cos_theta;
	double	sin_theta;
	t_v3	local;
	t_v3	tangent;
	t_v3	bitangent;
	double	a2;

	u1 = tls_random_double();
	u2 = tls_random_double();
	a2 = roughness * roughness;
	phi = 2.0 * M_PI * u1;
	cos_theta = sqrt((1.0 - u2) / (1.0 + (a2 - 1.0) * u2));
	sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	local.x = sin_theta * cos(phi);
	local.y = sin_theta * sin(phi);
	local.z = cos_theta;
	build_tangent_frame(normal, &tangent, &bitangent);
	a = local.x;
	local = vadd(vadd(vmul(a, tangent), vmul(local.y, bitangent)),
			vmul(local.z, normal));
	return (normalize(local));
}

static void	schlick_fresnel_rgb(t_rgb f0, double cos_theta, double *fr,
		double *fg, double *fb)
{
	double	t;

	t = 1.0 - cos_theta;
	t = t * t;
	t = t * t;
	t = t * (1.0 - cos_theta);
	*fr = f0.r / 255.0 + (1.0 - f0.r / 255.0) * t;
	*fg = f0.g / 255.0 + (1.0 - f0.g / 255.0) * t;
	*fb = f0.b / 255.0 + (1.0 - f0.b / 255.0) * t;
}

t_rgb	metallic_ray(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	new_ray;
	t_v3	v;
	t_v3	h;
	t_v3	l;
	double	ndv;
	double	voh;
	double	nh;
	double	rough;
	double	fr;
	double	fg;
	double	fb;
	t_rgb	trace_color;
	t_rgb	result;
	int		attempts;

	if (data->sample_mode)
	{
		new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
		new_ray.direction = reflect(ray->direction, ray->normal);
		trace_color = path_trace(&new_ray, data, depth - 1);
		result.r = (unsigned char)fmin(trace_color.r
				* closest->material.reflectivity, 255);
		result.g = (unsigned char)fmin(trace_color.g
				* closest->material.reflectivity, 255);
		result.b = (unsigned char)fmin(trace_color.b
				* closest->material.reflectivity, 255);
		return (result);
	}
	rough = closest->material.roughness;
	if (rough < 0.01)
		rough = 0.01;
	v = vmul(-1.0, ray->direction);
	ndv = dot(ray->normal, v);
	if (ndv <= 0)
		return (rgbdefine(0, 0, 0));
	attempts = 0;
	while (attempts < 16)
	{
		double	ndl;
		double	k;
		double	g1v;
		double	g1l;
		double	g;
		double	weight;

		h = ggx_sample(ray->normal, rough);
		l = vsub(vmul(2.0 * dot(v, h), h), v);
		ndl = dot(ray->normal, l);
		voh = dot(v, h);
		nh = dot(ray->normal, h);
		attempts++;
		if (ndl <= 0 || voh <= 0 || nh <= 0)
			continue ;
		k = rough * rough * 0.5;
		g1v = ndv / (ndv * (1.0 - k) + k);
		g1l = ndl / (ndl * (1.0 - k) + k);
		g = g1v * g1l;
		weight = g * voh / (ndv * nh);
		schlick_fresnel_rgb(closest->rgb, voh, &fr, &fg, &fb);
		new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
		new_ray.direction = l;
		trace_color = path_trace(&new_ray, data, depth - 1);
		result.r = (unsigned char)fmin(trace_color.r * fr * weight
				* METAL_BOOST, 255);
		result.g = (unsigned char)fmin(trace_color.g * fg * weight
				* METAL_BOOST, 255);
		result.b = (unsigned char)fmin(trace_color.b * fb * weight
				* METAL_BOOST, 255);
		return (result);
	}
	new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	new_ray.direction = reflect(ray->direction, ray->normal);
	trace_color = path_trace(&new_ray, data, depth - 1);
	schlick_fresnel_rgb(closest->rgb, fabs(dot(ray->direction, ray->normal)),
		&fr, &fg, &fb);
	result.r = (unsigned char)fmin(trace_color.r * fr * METAL_BOOST, 255);
	result.g = (unsigned char)fmin(trace_color.g * fg * METAL_BOOST, 255);
	result.b = (unsigned char)fmin(trace_color.b * fb * METAL_BOOST, 255);
	return (result);
}
