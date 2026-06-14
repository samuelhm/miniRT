/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcs_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 11:58:20 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

uint32_t	texture_weight(t_rgb c1, t_rgb c2)
{
	c1.r *= 0.9;
	c1.g *= 0.9;
	c1.b *= 0.9;
	c2.r *= 0.1;
	c2.g *= 0.1;
	c2.b *= 0.1;
	return (get_colour(color_add(c1, c2)));
}

t_rgb	path_trace_type(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_rgb	res;

	res = rgbdefine(0, 0, 0);
	if (closest->material.m_type == -1)
		res = diffuse_ray(ray, closest, data, depth);
	else if (closest->material.m_type == MT)
		res = metallic_ray(ray, closest, data, depth);
	else if (closest->material.m_type == MR)
		res = mirror_ray(ray, closest, data, depth);
	else if (closest->material.m_type == GL)
		res = glass_ray(ray, closest, data, depth);
	return (res);
}
