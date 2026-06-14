/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 15:27:23 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	console_get_k(t_obj *obj, int i)
{
	int		k;

	k = i;
	if (obj->material.m_type == MR && i == 5)
		k += 5;
	else if (obj->material.m_type == MT && i == 6)
		k += 5;
	else if (obj->material.m_type == GL && i == 7)
		k += 5;
	else if (obj->material.m_type == EM && i == 8)
		k += 5;
	else if ((obj->material.m_type < 0 || obj->material.m_type > 4 \
							|| obj->material.m_type == 3) && i == 4)
		k = 9;
	return (k);
}

void	put_img_buttons(t_data *data)
{
	t_obj		*obj;
	mlx_image_t	**icons;
	int			i;
	int			k;

	obj = data->console->last_item;
	icons = data->console->icons;
	i = 3;
	if (data->console->last_type != OBJ)
		return ;
	while (++i < 9)
	{
		k = console_get_k(obj, i);
		mlx_image_to_window(data->mlx, icons[k], data->x - \
								(280) + (58 * (i - 4)), 465);
		lst_add_back(&data->console->btn_list, \
						lst_new(data->console->icons[k]));
		mlx_set_instance_depth(&data->console->icons[k]->instances[0], 3);
	}
}
