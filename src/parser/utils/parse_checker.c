/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:18:36 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 16:37:40 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	check_end(t_data *data, char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	i = skip_color(data, str, i, 0);
	i = skip_color(data, str, i, 0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && str[i] != '\n')
		if (!ft_isspace(str[i++]))
			exit(er(data, "error: check_end: char after last num", str));
}

void	check_params_acl(t_data *data, t_cam *cam)
{
	if (!data->a_light && !data->s_light)
		exit(er(data, "error: check_params_acl: light needed", NULL));
	if (!data->a_light)
		data->a_light = calloc(1, sizeof(t_alight));
	if (!cam)
		exit(er(data, "error: check_params_acl: ACL left", NULL));
	if (cam->axis.x < -1 || cam->axis.x > 1)
		exit(er(data, "error: check_params_acl: cam axis.x 0-1", NULL));
	else if (cam->axis.y < -1 || cam->axis.y > 1)
		exit(er(data, "error: check_params_acl: cam axis.y 0-1", NULL));
	else if (cam->axis.z < -1 || cam->axis.z > 1)
		exit(er(data, "error: check_params_acl: cam axis.z 0-1", NULL));
	else if (cam->fov < 0)
		exit(er(data, "error: check_params_acl: cam fov <0", NULL));
}

void	check_obj_normi(t_obj *obj)
{
	if (obj->axis.x < -1 || obj->axis.x > 1)
		exit(er(obj->data, "error: check_obj_normi: axis.x 0-1", NULL));
	else if (obj->axis.y < -1 || obj->axis.y > 1)
		exit(er(obj->data, "error: check_obj_normi: axis.y 0-1", NULL));
	else if (obj->axis.z < -1 || obj->axis.z > 1)
		exit(er(obj->data, "error: check_obj_normi: axis.z 0-1", NULL));
	else if ((obj->type == CY || obj->type == CO) && obj->height < 1)
		exit(er(obj->data, "error: check_obj_normi: height <1", NULL));
	else if (obj->type == CO && (obj->size > 90 || obj->size < 1))
		exit(er(obj->data, "error: check_obj_normi: size <1", NULL));
	if (obj->type == CU)
	{
		if (obj->cube_size.x < 1)
			exit(er(obj->data, "error: check_obj_normi: size x <1", NULL));
		else if (obj->cube_size.y < 1)
			exit(er(obj->data, "error: check_obj_normi: size y <1", NULL));
		else if (obj->cube_size.z < 1)
			exit(er(obj->data, "error: check_obj_normi: size z <1", NULL));
	}
}

void	check_obj(t_obj *obj)
{
	while (obj)
	{
		if (obj->type != PL && obj->type != CO && obj->size <= 0)
			exit(er(obj->data, "error: check_obj: sp/cy size <=0", NULL));
		else if (obj->type != SP)
			check_obj_normi(obj);
		if (obj->type == CO && (obj->size < 0 || obj->size > 54))
			exit(er(obj->data, "error: check_obj: co size 0-54", NULL));
		else if (obj->type == CO)
			obj->size += 35;
		obj = obj->next;
	}
}
