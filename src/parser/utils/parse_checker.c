/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:18:36 by erigonza          #+#    #+#             */
/*   Updated: 2024/12/19 16:04:45 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
// #include "threads.h"

void	check_end(char *str, int i)
{
	while (str[i] && ft_isspace(str[i]))
		i++;
	i = skip_color(str, i, 0);
	i = skip_color(str, i, 0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && str[i] != '\n')
		if (!ft_isspace(str[i++]))
			exit(er("error: check_end: char after last num", str));
}

void	check_light(t_alight *aL, t_slight *sL)
{
	if (aL)
	{
		if (aL->br < 0)
			exit(er("error: check_lihgt: brightness < 0", NULL));
		if (aL->rgb.r < 0 || aL->rgb.r > 255)
			exit(er("error: check_lihgt: aL rgb.r <0 / >255", NULL));
		else if (aL->rgb.g < 0 || aL->rgb.g > 255)
			exit(er("error: check_lihgt: aL rgb.g <0 / >255", NULL));
		else if (aL->rgb.b < 0 || aL->rgb.b > 255)
			exit(er("error: check_lihgt: aL rgb.b <0/>255", NULL));
	}
	while (sL)
	{
		if (sL->br < 0)
			exit(er("error: ACL left", NULL));
		else if (sL->rgb.r < 0 || sL->rgb.r > 255)
			exit(er("error: sL rgb.r <0 / >255", NULL));
		else if (sL->rgb.g < 0 || sL->rgb.g > 255)
			exit(er("error: sL rgb.g <0 / >255", NULL));
		else if (sL->rgb.b < 0 || sL->rgb.b > 255)
			exit(er("error: sL rgb.b <0/>255", NULL));
		sL = sL->next;
	}
}

void	check_params_acl(t_alight *aL, t_slight *sL, t_cam *cam)
{
	if ((!aL && !sL) || !cam)
		exit(er("error: check_params_acl: ACL left", NULL));
	check_light(aL, sL);
	if (cam->axis.x < -1 || cam->axis.x > 1)
		exit(er("error: check_params_acl: cam axis.x <0 / <1", NULL));
	else if (cam->axis.y < -1 || cam->axis.y > 1)
		exit(er("error: check_params_acl: cam axis.y <0 / <1", NULL));
	else if (cam->axis.z < -1 || cam->axis.z > 1)
		exit(er("error: check_params_acl: cam axis.z <0 / <1", NULL));
	else if (cam->fov < 0)
		exit(er("error: check_params_acl: cam fov <0", NULL));
}

void	check_obj(t_obj *obj)
{
	while (obj)
	{
		if (obj->rgb.r < 0 || obj->rgb.r > 255)
			exit(er("error: check_obj: rgb.r <0 / >255", NULL));
		else if (obj->rgb.g < 0 || obj->rgb.g > 255)
			exit(er("error: check_obj: rgb.g <0 / >255", NULL));
		else if (obj->rgb.b < 0 || obj->rgb.b > 255)
			exit(er("error: check_obj: rgb.b <0/>255", NULL));
		else if (obj->type != PL && obj->size <= 0)
			exit(er("error: check_obj: sp/cy size <= 0", NULL));
		else if (obj->type != SP)
		{
			if (obj->axis.x < -1 || obj->axis.x > 1)
				exit(er("error: check_obj: pl/cy axis.x <0 / <1", NULL));
			else if (obj->axis.y < -1 || obj->axis.y > 1)
				exit(er("error: check_obj: pl/cy axis.y <0 / <1", NULL));
			else if (obj->axis.z < -1 || obj->axis.z > 1)
				exit(er("error: check_obj: pl/cy axis.z <0 / <1", NULL));
			else if (obj->type == CY && obj->height < 0)
				exit(er("error: check_obj: cy height < 0", NULL));
		}
		obj = obj->next;
	}
}

void	check_params(t_data *data)
{
	t_obj	*obj;

	obj = data->obj;
	if (!obj)
		exit(er("erro: check_params: no obj detected", NULL));
	check_obj(obj);
	check_params_acl(data->a_light, data->s_light, data->cam);
}
