/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_labels_set_obj.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:17:22 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:28:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	plane_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_v3	axis;
	t_obj	*obj;

	obj = data->console->last_item;
	pos = obj->pos;
	axis = obj->axis;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "x           %.2f", axis.x);
	snprintf(img_btn->row5, size, "y           %.2f", axis.y);
	snprintf(img_btn->row6, size, "z           %.2f", axis.z);
}

void	sphere_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_obj	*obj;

	obj = data->console->last_item;
	pos = obj->pos;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "              %.0f", obj->size);
}

void	cylinder_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_v3	axis;
	t_obj	*obj;

	obj = data->console->last_item;
	pos = obj->pos;
	axis = obj->axis;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "x           %.2f", axis.x);
	snprintf(img_btn->row5, size, "y           %.2f", axis.y);
	snprintf(img_btn->row6, size, "z           %.2f", axis.z);
	snprintf(img_btn->row7, size, "Width         %.0f", obj->size);
	snprintf(img_btn->row8, size, "Height        %.0f", obj->height);
}

void	cube_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_v3	axis;
	t_obj	*obj;

	obj = data->console->last_item;
	pos = obj->pos;
	axis = obj->axis;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "x           %.2f", axis.x);
	snprintf(img_btn->row5, size, "y           %.2f", axis.y);
	snprintf(img_btn->row6, size, "z           %.2f", axis.z);
	snprintf(img_btn->row7, size, "x             %.0f", obj->cube_size.x);
	snprintf(img_btn->row8, size, "y             %.0f", obj->cube_size.y);
	snprintf(img_btn->row9, size, "z             %.0f", obj->cube_size.z);
}

void	cone_labels(t_data *data, t_img_btn *img_btn)
{
	int		size;
	t_v3	pos;
	t_v3	axis;
	t_obj	*obj;

	obj = data->console->last_item;
	pos = obj->pos;
	axis = obj->axis;
	size = sizeof(img_btn->row1);
	snprintf(img_btn->row1, size, "x           %.0f", pos.x);
	snprintf(img_btn->row2, size, "y           %.0f", pos.y);
	snprintf(img_btn->row3, size, "z           %.0f", pos.z);
	snprintf(img_btn->row4, size, "x           %.2f", axis.x);
	snprintf(img_btn->row5, size, "y           %.2f", axis.y);
	snprintf(img_btn->row6, size, "z           %.2f", axis.z);
	snprintf(img_btn->row7, size, "Width         %.0f", obj->size);
	snprintf(img_btn->row8, size, "Height        %.0f", obj->height);
}
