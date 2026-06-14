/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:55:14 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 09:52:26 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_v2	calculate_uv(t_v3 point, t_obj *obj)
{
	t_v2	result;

	result.u = -1;
	result.v = -1;
	if (obj->type == SIDE)
		return (side_uv(point, obj));
	else if (obj->type == CY)
		return (cylinder_uv(point, obj));
	else if (obj->type == SP)
		return (sphere_uv(point, obj));
	else if (obj->type == PL)
		return (plane_uv(point, obj));
	else if (obj->type == CO)
		return (cone_uv(point, obj));
	else
		return (result);
}

t_rgb	texture_color(t_obj *obj, t_v2 uv)
{
	int				x;
	int				y;
	uint32_t		index;
	t_rgb			rgbc;

	x = (int)(uv.u * obj->material.texture->width) \
		% (int)obj->material.texture->width;
	y = (int)(uv.v * obj->material.texture->height) \
		% (int)obj->material.texture->height;
	index = (uint32_t)(y * (int)obj->material.texture->width + x) * 4u;
	rgbc.r = obj->material.texture->pixels[index];
	rgbc.g = obj->material.texture->pixels[index + 1];
	rgbc.b = obj->material.texture->pixels[index + 2];
	return (rgbdefine(rgbc.r, rgbc.g, rgbc.b));
}
