/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_labels_set.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 20:05:19 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:28:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	write_cam_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_v3	axis;

	pos = data->cam->pos;
	axis = data->cam->axis;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x          %.2f", pos.x);
	snprintf(img_btn->row2, size, "y          %.2f", pos.y);
	snprintf(img_btn->row3, size, "z          %.2f", pos.z);
	snprintf(img_btn->row4, size, "x           %.2f", axis.x);
	snprintf(img_btn->row5, size, "y           %.2f", axis.y);
	snprintf(img_btn->row6, size, "z           %.2f", axis.z);
	snprintf(img_btn->row7, size, "FOV           %d", data->cam->fov);
	snprintf(img_btn->row8, size, "BLUR         %.2f", data->cam->aperture);
	snprintf(img_btn->row9, size, "BL_DIST       %.0f", data->cam->focus_dist);
}

void	write_alight_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	float	br;

	br = data->a_light->br;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "br          %.2f", br);
}

void	write_slight_labels(t_data *data, t_img_btn *img_btn)
{
	int			size;
	t_v3		pos;
	float		br;
	t_slight	*slight;

	slight = data->console->last_item;
	br = slight->br;
	pos = slight->pos;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "br          %.2f", br);
}

void	write_obj_labels(t_data *data, t_img_btn *img_btn)
{
	t_obj	*obj;

	obj = data->console->last_item;
	if (obj->type == PL)
		plane_labels(data, img_btn);
	else if (obj->type == SP)
		sphere_labels(data, img_btn);
	else if (obj->type == CY)
		cylinder_labels(data, img_btn);
	else if (obj->type == CU)
		cube_labels(data, img_btn);
	else if (obj->type == CO)
		cone_labels(data, img_btn);
}
