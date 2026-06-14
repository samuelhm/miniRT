/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_init_general.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:51:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 19:24:23 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	click_obj_console(t_data *data, t_btn_name clicked)
{
	t_obj	*obj;

	obj = data->console->last_item;
	if (obj_click_move(data, obj, clicked))
	{
		run_console(data);
		return ;
	}
	if (clicked == pos_xmin)
		obj->pos.x -= OBJPLUS;
	else if (clicked == pos_xmax)
		obj->pos.x += OBJPLUS;
	else if (clicked == pos_ymin)
		obj->pos.y -= OBJPLUS;
	else if (clicked == pos_ymax)
		obj->pos.y += OBJPLUS;
	else if (clicked == pos_zmin)
		obj->pos.z -= OBJPLUS;
	else if (clicked == pos_zmax)
		obj->pos.z += OBJPLUS;
	else
		click_type(data, obj, clicked, obj->type);
	free_after_click(data);
	init_all(data);
	data->render_sel = render_fast;
}

void	init_console_normi(t_data *data)
{
	t_obj	*obj;

	if (data->console->last_type == OBJ)
	{
		obj = data->console->last_item;
		if (obj->type != SP)
		{
			rows_5(data);
			rows_6(data);
			if (obj->type != PL)
			{
				rows_7(data);
				rows_8(data);
				if (obj->type == CU)
					rows_9(data);
			}
		}
	}
	if (data->console->last_type == OBJ)
		row_material(data, btn_df);
}

void	init_console(t_data *data)
{
	make_arrows(data);
	rows_1(data);
	if (data->console->last_type == ALIGHT)
		return ;
	rows_2(data);
	rows_3(data);
	rows_4(data);
	if (data->console->last_type == SLIGHT)
		return ;
	if (data->console->last_type == CAM)
	{
		rows_5(data);
		rows_6(data);
		rows_7(data);
		rows_8(data);
		rows_9(data);
	}
	init_console_normi(data);
}
