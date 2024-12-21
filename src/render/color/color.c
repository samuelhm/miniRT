/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:19:01 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/16 13:09:02 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

// From 0-255 BRGA colour Returns uint32 colour
uint32_t	get_acolour(t_uchar alpha, t_uchar r, t_uchar g, t_uchar b)
{
	return ((alpha << 24) | (b << 16) | (g << 8) | r);
}

uint32_t	get_colour(t_rgb color)
{
	return (get_acolour(255, color.r, color.g, color.b));
}