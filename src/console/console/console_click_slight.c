/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_click_slight.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 11:07:30 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static bool	slight_click_move_normi(t_data *data)
{
	if (data->console->last_item == data->s_light)
	{
		data->console->last_item = NULL;
		if (data->a_light->exist)
			data->console->last_type = ALIGHT;
		else
			data->console->last_type = CAM;
	}
	else
		data->console->last_item = get_prev(data, data->console->last_item);
	return (true);
}

bool	slight_click_move(t_data *data, t_slight *slight, t_btn_name clicked)
{
	if (clicked == left && slight_click_move_normi(data))
		return (true);
	else if (clicked == right)
	{
		if (slight->next)
		{
			data->console->last_item = slight->next;
			data->console->last_type = SLIGHT;
			return (true);
		}
		else if (data->obj)
		{
			data->console->last_item = data->obj;
			data->console->last_type = OBJ;
			return (true);
		}
	}
	return (false);
}

void	click_slight_console(t_data *data, t_slight *slight, \
										t_btn_name clicked)
{
	if (slight_click_move(data, slight, clicked))
	{
		run_console(data);
		return ;
	}
	if (clicked == pos_xmin)
		slight->pos.x -= SPOTPLUS;
	else if (clicked == pos_xmax)
		slight->pos.x += SPOTPLUS;
	else if (clicked == pos_ymin)
		slight->pos.y -= SPOTPLUS;
	else if (clicked == pos_ymax)
		slight->pos.y += SPOTPLUS;
	else if (clicked == pos_zmin)
		slight->pos.z -= SPOTPLUS;
	else if (clicked == pos_zmax)
		slight->pos.z += SPOTPLUS;
	else if (clicked == row4l)
		slight->br = fmaxf(0, slight->br - LIGHTPLUS);
	else if (clicked == row4r)
		slight->br = fminf(1, slight->br + LIGHTPLUS);
	init_light(data);
	data->render_sel = render_fast;
}
