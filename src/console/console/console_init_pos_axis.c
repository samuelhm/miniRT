/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_init_pos_axis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:51:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 19:21:54 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	make_arrows(t_data *data)
{
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	btn->name = left;
	btn->x = data->console->icons[0]->instances[0].x;
	btn->y = data->console->icons[0]->instances[0].y;
	btn->h = data->console->icons[0]->height;
	btn->w = data->console->icons[0]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	btn->name = right;
	btn->x = data->console->icons[1]->instances[0].x;
	btn->y = data->console->icons[1]->instances[0].y;
	btn->h = data->console->icons[1]->height;
	btn->w = data->console->icons[1]->width;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_1(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	tmp.name = row1l;
	tmp.x = data->console->icons[2]->instances[0].x;
	tmp.y = data->console->icons[2]->instances[0].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	tmp.name = row1r;
	tmp.x = data->console->icons[3]->instances[0].x;
	tmp.y = data->console->icons[3]->instances[0].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_2(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	tmp.name = row2l;
	tmp.x = data->console->icons[2]->instances[1].x;
	tmp.y = data->console->icons[2]->instances[1].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	tmp.name = row2r;
	tmp.x = data->console->icons[3]->instances[1].x;
	tmp.y = data->console->icons[3]->instances[1].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_3(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	tmp.name = row3l;
	tmp.x = data->console->icons[2]->instances[2].x;
	tmp.y = data->console->icons[2]->instances[2].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	tmp.name = row3r;
	tmp.x = data->console->icons[3]->instances[2].x;
	tmp.y = data->console->icons[3]->instances[2].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}

void	rows_4(t_data *data)
{
	t_btn	tmp;
	t_btn	*btn;

	btn = calloc(1, sizeof(t_btn));
	tmp.name = row4l;
	tmp.x = data->console->icons[2]->instances[3].x;
	tmp.y = data->console->icons[2]->instances[3].y;
	tmp.h = data->console->icons[2]->height;
	tmp.w = data->console->icons[2]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
	btn = calloc(1, sizeof(t_btn));
	tmp.name = row4r;
	tmp.x = data->console->icons[3]->instances[3].x;
	tmp.y = data->console->icons[3]->instances[3].y;
	tmp.h = data->console->icons[3]->height;
	tmp.w = data->console->icons[3]->width;
	*btn = tmp;
	lst_add_front(&data->console->click_lst, lst_new(btn));
}
