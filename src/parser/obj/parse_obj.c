/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_obj.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:02:22 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 12:06:24 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	create_obj_normi(t_obj *obj, char **tmp, char **tmp2)
{
	obj->material.emision = -1;
	if (obj->type == SP)
	{
		obj->size = parse_atof(obj->data, *tmp, 0);
		*tmp2 = str_sub(*tmp, (unsigned int)sum_parse(obj->data, *tmp, 0, 0), \
											strlen(*tmp));
	}
	else if (obj->type == CY || obj->type == CO || obj->type == CU)
	{
		obj->size = parse_atof(obj->data, *tmp2, 0);
		obj->cube_size.x = obj->size;
		obj->i = sum_parse(obj->data, *tmp2, 0, 0);
		obj->height = parse_atof(obj->data, *tmp2, obj->i);
		obj->cube_size.y = obj->height;
		obj->i = sum_parse(obj->data, *tmp2, obj->i, 0);
		free(*tmp);
		*tmp = str_sub(*tmp2, (unsigned int)obj->i, strlen(*tmp2));
	}
	if (obj->type == CU)
	{
		obj->cube_size.z = parse_atof(obj->data, *tmp, 0);
		free(*tmp2);
		obj->i = skip_double(obj->data, *tmp, 0, 0);
		*tmp2 = str_sub(*tmp, (unsigned int)obj->i, strlen(*tmp));
	}
}

t_obj	*create_obj(t_data *data, char *str, int type)
{
	char	*tmp;
	char	*tmp2;
	char	*target;
	t_obj	*obj;

	obj = new_obj(data);
	obj->data = data;
	obj->type = type;
	tmp = doubles_parse(obj, str, 2, 0);
	if (obj->type != SP)
		tmp2 = doubles_parse(obj, tmp, 0, 1);
	create_obj_normi(obj, &tmp, &tmp2);
	if (obj->type != CY && obj->type != CO)
		target = tmp2;
	else
		target = tmp;
	obj->rgb = colors_parse(data, target, 0);
	obj->material.texture = NULL;
	obj->material.bm_texture = NULL;
	extra_functionalities(obj, target);
	if (tmp2)
		free(tmp2);
	return (free(tmp), obj);
}
