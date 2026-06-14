/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_obj_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:31:28 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 12:32:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_set_prev(t_data *data)
{
	t_obj		*obj;

	obj = data->obj;
	data->obj->prev = NULL;
	while (obj)
	{
		if (obj->next)
			obj->next->prev = obj;
		obj = obj->next;
	}
}

void	init_obj_normi(t_data *data, t_obj *obj)
{
	obj->calcs.radius = obj->size * 0.5f;
	obj->calcs.radius2 = obj->calcs.radius * obj->calcs.radius;
	if (obj->type != SP)
		obj->axis = normalize(obj->axis);
	set_calcs(data, obj);
	if (obj->material.board_scale != -1)
		obj->material.rgb_checker = \
			apply_al(obj->material.rgb_checker, data->a_light);
	obj->calcs.caps_normal = vmul(-1.0f, obj->axis);
	obj->calcs.half_angle = (obj->size * 0.5f) * (M_PI / 180.0f);
	obj->calcs.cos_half = cos(obj->calcs.half_angle);
	obj->calcs.k = obj->calcs.cos_half * obj->calcs.cos_half;
	if (obj->type == PL || obj->type == SIDE || obj->type == CAP)
		obj->calcs.numerator = dot(vsub(obj->pos, data->cam->pos), obj->axis);
	obj->calcs.half_size = vmul(0.5, obj->cube_size);
	if (obj->type == CO)
	{
		obj->calcs.oc = vsub(data->cam->pos, obj->pos);
		obj->calcs.dot_oc_axis = dot(obj->calcs.oc, obj->axis);
	}
}
