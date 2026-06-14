/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_board.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 10:40:12 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 12:26:35 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"

t_rgb	checkerboard_color(t_obj *obj, t_v3 point)
{
	t_v3	local_pos;
	t_v3	tangent;
	t_v3	bitangent;
	double	u;
	double	v;
	int		x;
	int		y;

	local_pos = vsub(point, obj->pos);
	if (fabs(obj->axis.y) < fabs(obj->axis.x))
		tangent = vdefine(0, 1, 0);
	else
		tangent = vdefine(1, 0, 0);
	bitangent = cross(obj->axis, tangent);
	u = dot(local_pos, tangent) * obj->material.board_scale;
	v = dot(local_pos, bitangent) * obj->material.board_scale;
	x = (int)floor(u);
	y = (int)floor(v);
	if ((x + y) % 2 == 0)
		return (obj->a_rgb);
	else
		return (obj->material.rgb_checker);
}
