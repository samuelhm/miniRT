/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colour.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 12:19:01 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/01 11:57:11 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"

// From 0-255 ARGB colour Returns uint32 colour
uint32_t	get_acolour(t_uchar alpha, t_uchar r, t_uchar g, t_uchar b)
{
	return ((alpha << 24) | (r << 16) | (g << 8) | b);
}


/**
 * Calcula el color final del objeto aplicando iluminación ambiental.
 * @obj_color: Color base del objeto (antes de la iluminación).
 * @ambient_light: Puntero a la luz ambiental que contiene el brillo y el color.
 *
 * Esta función ajusta el color del objeto en función de la luz ambiental.
 * La iluminación ambiental se calcula multiplicando el color base del objeto
 * por el brillo de la luz ambiental y sumando el color de la luz ambiental
 * ajustado por el mismo brillo. Los valores resultantes de cada canal (R, G, B)
 * se limitan al rango [0, 255] para evitar desbordamientos (con fminf).
 *
 * Retorna el color ajustado (t_rgb) con la iluminación ambiental aplicada.
 */
t_rgb	apply_ambient_light(t_rgb obj_color, t_aLight *ambient_light)
{
	t_rgb	result;
	float	bright;

	bright = ambient_light->br;
	result.r = fminf(obj_color.r * bright + ambient_light->rgb.r * bright, 255);
	result.g = fminf(obj_color.g * bright + ambient_light->rgb.g * bright, 255);
	result.b = fminf(obj_color.b * bright + ambient_light->rgb.b * bright, 255);

	return (result);
}
