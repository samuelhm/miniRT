/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calcs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/01 13:00:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"
#include "../lib/libvector/libvct.h"
#include "../inc/miniRT.h"

/**
 * solve_quadratic - Resuelve una ecuación cuadrática en la forma ax^2 + bx + c = 0.
 * @quad: Puntero a la estructura t_quadratic que contiene los coeficientes
 *        a, b, c y donde se almacenarán las soluciones t1 y t2.
 *
 * Esta función calcula el discriminante de la ecuación cuadrática (b^2 - 4ac).
 * Si el discriminante es negativo, no hay soluciones reales, y la función
 * retorna false. Si el discriminante es no negativo, calcula las dos raíces t1 y t2
 * y las almacena en la estructura t_quadratic.
 *
 * Retorna true si la ecuación tiene soluciones reales, o false si no las tiene.
 * t1 y t2 representa cuando el rayo entra y sale del objeto, si solo hay t2,
 * significa que el rayo empezó dentro del objeto, y solo esta el
 * impacto de salida
 */
bool	solve_quadratic(t_quadratic *quad)
{
	float	sqrt_discriminant;

	if (quad->discriminant < 0)
		return (false);
	sqrt_discriminant = sqrtf(quad->discriminant);
	quad->t1 = (-quad->b - sqrt_discriminant) / (2.0f * quad->a);
	quad->t2 = (-quad->b + sqrt_discriminant) / (2.0f * quad->a);
	return (true);
}

/**
 * trace_ray - Determina el color del píxel intersectado por un rayo.
 * @ray: Puntero al rayo que se desea trazar.
 * @scene: Puntero a la estructura que representa la escena (objetos, luces, etc.).
 *
 * Esta función recorre todos los objetos en la escena para determinar cuál es
 * el más cercano que intersecta el rayo. Si se encuentra un objeto, ajusta su
 * color base utilizando la iluminación ambiental de la escena. Si no hay intersección,
 * retorna un color de fondo (negro por defecto).
 *
 * Retorna un t_rgb que representa el color del píxel correspondiente al rayo.
 */
t_rgb	trace_ray(t_ray *ray, t_data *scene)
{
	t_obj	*obj;
	float	t_min;
	t_obj	*closest_obj;
	t_rgb	background;
	float	t;

	obj = scene->obj;
	t_min = INFINITY;
	closest_obj = NULL;
	background.r = 0;
	background.g = 0;
	background.b = 0;
	while (obj)
	{
		if (obj->type == SP)
		{
			t = intersect_sphere(ray, obj, &t);
			if (t > 0 && t < t_min)
			{
				t_min = t;
				closest_obj = obj;
			}
		}
		obj = obj->next;
	}
	if (closest_obj)
		return (apply_ambient_light(closest_obj->rgb, scene->aLight));
	return (background);
}
