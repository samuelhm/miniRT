/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_click_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:49:18 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:28:30 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	obj_remove(t_obj **lst, t_obj *node)
{
	t_obj	*temp;
	t_obj	*lstp;

	if (!lst || !(*lst) || !node)
		return (0);
	if (*lst == node)
	{
		temp = *lst;
		*lst = (*lst)->next;
		free(temp);
		temp = NULL;
		return (1);
	}
	lstp = *lst;
	while (lstp && lstp->next != node)
		lstp = lstp->next;
	if (lstp->next == node)
	{
		temp = lstp->next;
		lstp->next = temp->next;
		free(temp);
		temp = NULL;
		return (1);
	}
	return (0);
}

void	free_after_click(t_data *data)
{
	t_obj		*obj;
	t_obj		*next;

	obj = data->obj;
	while (obj)
	{
		next = obj->next;
		if (obj && (obj->type == SIDE || obj->type == CAP))
		{
			obj_remove(&data->obj, obj);
		}
		obj = next;
	}
}

t_btn_name	get_clicked_btn(t_data *data, int x, int y)
{
	t_list	*click;
	t_btn	*btn;

	click = data->console->click_lst;
	while (click)
	{
		btn = click->content;
		if (x > btn->x && x < btn->x + btn->w)
			if (y > btn->y && y < btn->y + btn->h)
				return (btn->name);
		click = click->next;
	}
	return (none);
}

t_slight	*get_prev(t_data *data, t_slight *slight)
{
	t_slight	*prev;

	prev = data->s_light;
	while (prev)
	{
		if (prev->next == slight)
			return (prev);
		prev = prev->next;
	}
	return (NULL);
}
