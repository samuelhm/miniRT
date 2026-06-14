/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consol_init_rows.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:51:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 19:19:26 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	rows_5(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	if (!btn)
		return ;
	tmp.name = row5l;
	tmp.x = data->console->icons[2]->instances[4].x;
	tmp.y = data->console->icons[2]->instances[4].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	if (!btn)
		return ;
	tmp.name = row5r;
	tmp.x = data->console->icons[3]->instances[4].x;
	tmp.y = data->console->icons[3]->instances[4].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_6(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	if (!btn)
		return ;
	tmp.name = row6l;
	tmp.x = data->console->icons[2]->instances[5].x;
	tmp.y = data->console->icons[2]->instances[5].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	if (!btn)
		return ;
	tmp.name = row6r;
	tmp.x = data->console->icons[3]->instances[5].x;
	tmp.y = data->console->icons[3]->instances[5].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}
