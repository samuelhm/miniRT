/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_click_alight.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 10:01:57 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

bool	alight_click_move(t_data *data, t_btn_name clicked)
{
	if (clicked == left)
	{
		data->console->last_type = CAM;
		run_console(data);
		return (true);
	}
	else if (clicked == right)
	{
		if (data->s_light)
		{
			data->console->last_item = data->s_light;
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

void	click_alight_console(t_data *data, t_btn_name clicked)
{
	if (alight_click_move(data, clicked))
	{
		run_console(data);
		return ;
	}
	if (clicked == br_min)
		data->a_light->br = fmax(data->a_light->br - LIGHTPLUS, LIGHTMIN);
	else if (clicked == br_max)
		data->a_light->br = fmin(data->a_light->br + LIGHTPLUS, LIGHTMAX);
	free_after_click(data);
	init_all(data);
	data->render_sel = render_fast;
}
