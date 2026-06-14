/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:58:38 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/23 11:11:35 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	fill_image(t_data *data, uint32_t *pixels, uint32_t *img_rgb)
{
	memcpy(pixels, img_rgb, (size_t)data->y * data->x * sizeof(uint32_t));
}

void	set_last(t_data *data)
{
	if (data->last_render == ONE)
		data->render_sel = render_one;
	else if (data->last_render == FAST)
		data->render_sel = render_fast;
	else if (data->last_render == UPDATE)
		data->render_sel = update_render;
}
