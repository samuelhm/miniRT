/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_click_obj.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 13:23:37 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 15:53:57 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	click_sphere(t_data *data, t_obj *obj, t_btn_name clicked)
{
	(void)data;
	if (clicked == row4r)
		obj->size += OBJPLUS;
	else if (clicked == row4l)
		obj->size -= OBJPLUS;
}

void	click_cy_co(t_data *data, t_obj *obj, t_btn_name clicked)
{
	(void)data;
	if (clicked == row7r)
		obj->size += OBJPLUS;
	else if (clicked == row7l)
		obj->size = fmax(0.1, obj->size - OBJPLUS);
	else if (clicked == row8r)
		obj->height += OBJPLUS;
	else if (clicked == row8l)
		obj->height = fmax(1, obj->height - OBJPLUS);
}

void	click_cube(t_data *data, t_obj *obj, t_btn_name clicked)
{
	(void)data;
	if (clicked == row7r)
		obj->cube_size.x += OBJPLUS;
	else if (clicked == row7l)
		obj->cube_size.x -= OBJPLUS;
	else if (clicked == row8r)
		obj->cube_size.y += OBJPLUS;
	else if (clicked == row8l)
		obj->cube_size.y -= OBJPLUS;
	else if (clicked == row9r)
		obj->cube_size.z += OBJPLUS;
	else if (clicked == row9l)
		obj->cube_size.z -= OBJPLUS;
}

static void	click_material(t_obj *obj, t_btn_name clicked)
{
	if (clicked == btn_df)
		obj->material.m_type = -1;
	else if (clicked == btn_mr)
		obj->material.m_type = MR;
	else if (clicked == btn_mt)
		obj->material.m_type = MT;
	else if (clicked == btn_gl)
		obj->material.m_type = GL;
	else if (clicked == btn_em)
	{
		obj->material.m_type = EM;
		obj->material.emision = 0.85;
		obj->material.self_emision = obj->material.emision * 2.5;
	}
	if (clicked == btn_gl || clicked == btn_mr || clicked == btn_em)
	{
		obj->material.texture = NULL;
		obj->material.bm_texture = NULL;
	}
	if (obj->rgb.r == 0)
		obj->rgb = rgbdefine(100, 100, 100);
}

void	click_type(t_data *data, t_obj *obj, t_btn_name clicked, int type)
{
	if (type == SP)
		click_sphere(data, obj, clicked);
	else if (clicked == axis_xmin)
		obj->axis.x -= OBJAXISP;
	else if (clicked == axis_xmax)
		obj->axis.x += OBJAXISP;
	else if (clicked == axis_ymin)
		obj->axis.y -= OBJAXISP;
	else if (clicked == axis_ymax)
		obj->axis.y += OBJAXISP;
	else if (clicked == axis_zmin)
		obj->axis.z -= OBJAXISP;
	else if (clicked == axis_zmax)
		obj->axis.z += OBJAXISP;
	if (type == CY || type == CO)
		click_cy_co(data, obj, clicked);
	else if (type == CU)
		click_cube(data, obj, clicked);
	click_material(obj, clicked);
}
