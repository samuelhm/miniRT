/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:19:01 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/15 13:59:36 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_rgb	rgbdefine(t_uchar r, t_uchar g, t_uchar b)
{
	t_rgb	rgb;

	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
	return (rgb);
}

// From 0-255 BRGA colour Returns uint32 colour
uint32_t	get_acolour(t_uchar alpha, t_uchar r, t_uchar g, t_uchar b)
{
	return ((alpha << 24) | (b << 16) | (g << 8) | r);
}

uint32_t	get_colour(t_rgb color)
{
	return (get_acolour(255, color.r, color.g, color.b));
}

void	unpack(uint32_t color, t_rgb *s)
{
	s->b = (color >> 16) & 0xFF;
	s->g = (color >> 8) & 0xFF;
	s->r = (color) & 0xFF;
}

uint32_t	average(uint32_t c1, uint32_t c2)
{
	t_rgb	sample1;
	t_rgb	sample2;
	t_rgb	res;

	unpack(c1, &sample1);
	unpack(c2, &sample2);
	res.r = (sample1.r + sample2.r) * 0.5;
	res.g = (sample1.g + sample2.g) * 0.5;
	res.b = (sample1.b + sample2.b) * 0.5;
	return (get_colour(res));
}
