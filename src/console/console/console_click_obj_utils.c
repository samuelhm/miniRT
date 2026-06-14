/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_click_obj_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 13:23:37 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

bool	obj_click_move_normi(t_data *data, t_obj *obj, t_slight *slight)
{
	if (obj->prev)
		data->console->last_item = obj->prev;
	else
	{
		if (slight)
		{
			data->console->last_type = SLIGHT;
			data->console->last_item = slight;
		}
		else
			data->console->last_type = ALIGHT;
	}
	return (true);
}

bool	obj_click_move(t_data *data, t_obj *obj, t_btn_name clicked)
{
	t_slight	*slight;

	slight = data->s_light;
	while (slight->next)
		slight = slight->next;
	if (clicked == left)
	{
		if (obj_click_move_normi(data, obj, slight))
			return (true);
	}
	else if (clicked == right)
	{
		if (obj->next && (obj->next->type == SIDE || obj->next->type == CAP))
			return (false);
		else if (obj->next)
		{
			data->console->last_item = obj->next;
			return (true);
		}
	}
	return (false);
}
