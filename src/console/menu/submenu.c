/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   submenu.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 17:57:09 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/17 11:43:45 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	manage_alight(keys_t key, t_data *data)
{
	static float	quantity;

	if (quantity < 0.01f)
		quantity = ALIGHTPLUS;
	if (key == MLX_KEY_UP && data->a_light->br < ALIGHTMAX)
		data->a_light->br = fminf(data->a_light->br + quantity, ALIGHTMAX);
	else if (key == MLX_KEY_DOWN && data->a_light->br > ALIGHTMIN)
		data->a_light->br = fmaxf(data->a_light->br - quantity, ALIGHTMIN);
	else if (key == MLX_KEY_MINUS || key == MLX_KEY_KP_SUBTRACT)
		quantity = fmaxf(quantity - 0.01f, 0.01);
	else if (key == MLX_KEY_KP_ADD || key == MLX_KEY_EQUAL)
		quantity += 0.01f;
	print_alight_menu(data);
	printf("\nincrementos = %f\n", quantity);
}

void	manage_submenu(keys_t key, int option, t_data *data)
{
	if (option == 0)
		manage_alight(key, data);
	else if (option == 1)
		manage_cam(key, data);
	else if (option == 2)
		manage_spot(key, data);
	else if (option == 3)
		manage_obj(key, data);
}
