/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:19:01 by shurtado          #+#    #+#             */
/*   Updated: 2024/11/29 12:25:04 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/render.h"

// From 0-255 ARGB colour Returns uint32 colour
uint32_t	get_acolour(unsigned char alpha, unsigned char r, unsigned char g, unsigned char b)
{
	return ((alpha << 24) | (r << 16) | (g << 8) | b);
}
