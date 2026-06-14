/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_icon_count.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:43:07 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	count_arrows(t_data *data)
{
	t_obj	*obj;

	if (data->console->last_type == CAM)
		return (11);
	else if (data->console->last_type == ALIGHT)
		return (1);
	else if (data->console->last_type == SLIGHT)
		return (5);
	else if (data->console->last_type == OBJ)
	{
		obj = data->console->last_item;
		if (obj->type == PL)
			return (8);
		else if (obj->type == SP)
			return (5);
		else if (obj->type == CY || obj->type == CO)
			return (10);
		else if (obj->type == CU)
			return (11);
	}
	return (0);
}

int	count_labels(t_data *data)
{
	t_obj	*obj;

	obj = data->console->last_item;
	if (data->console->last_type == CAM)
		return (13);
	else if (data->console->last_type == ALIGHT)
		return (3);
	else if (data->console->last_type == SLIGHT)
		return (7);
	else if (data->console->last_type == OBJ)
	{
		if (obj->type == PL)
			return (9);
		else if (obj->type == SP)
			return (7);
		else if (obj->type == CY || obj->type == CO)
			return (12);
		else if (obj->type == CU)
			return (13);
	}
	return (0);
}
