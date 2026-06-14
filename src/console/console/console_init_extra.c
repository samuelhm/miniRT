/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_init_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:51:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:29:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	rows_7(t_data *data)
{
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	btn->name = row7l;
	btn->x = data->console->icons[2]->instances[6].x;
	btn->y = data->console->icons[2]->instances[6].y;
	btn->h = data->console->icons[2]->height;
	btn->w = data->console->icons[2]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	btn->name = row7r;
	btn->x = data->console->icons[3]->instances[6].x;
	btn->y = data->console->icons[3]->instances[6].y;
	btn->h = data->console->icons[3]->height;
	btn->w = data->console->icons[3]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_8(t_data *data)
{
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	btn->name = row8l;
	btn->x = data->console->icons[2]->instances[7].x;
	btn->y = data->console->icons[2]->instances[7].y;
	btn->h = data->console->icons[2]->height;
	btn->w = data->console->icons[2]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	btn->name = row8r;
	btn->x = data->console->icons[3]->instances[7].x;
	btn->y = data->console->icons[3]->instances[7].y;
	btn->h = data->console->icons[3]->height;
	btn->w = data->console->icons[3]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_9(t_data *data)
{
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	btn->name = row9l;
	btn->x = data->console->icons[2]->instances[8].x;
	btn->y = data->console->icons[2]->instances[8].y;
	btn->h = data->console->icons[2]->height;
	btn->w = data->console->icons[2]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	btn->name = row9r;
	btn->x = data->console->icons[3]->instances[8].x;
	btn->y = data->console->icons[3]->instances[8].y;
	btn->h = data->console->icons[3]->height;
	btn->w = data->console->icons[3]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	row_material(t_data *data, int click)
{
	int		i;
	int		k;
	t_obj	*obj;
	t_btn	*btn;

	i = 3;
	obj = data->console->last_item;
	if (data->console->last_type != OBJ)
		return ;
	while (++i < 9)
	{
		k = console_get_k(obj, i);
		btn = calloc(1, sizeof(t_btn));
		btn->name = click++;
		btn->x = data->console->icons[k]->instances[0].x;
		btn->y = data->console->icons[k]->instances[0].y;
		btn->h = data->console->icons[k]->height;
		btn->w = data->console->icons[k]->width;
		lst_add_front(&data->console->click_lst, lst_new(btn));
	}
}
