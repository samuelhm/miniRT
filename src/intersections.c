/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:48:44 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/01 11:57:36 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"
#include "../lib/libvector/libvct.h"
#include "../inc/miniRT.h"

//Return true if ray impacts on sphere object
bool	intersect_sphere(t_ray *ray, t_obj *sphere, float *t)
{
	t_p3		oc;
	t_quadratic	quad;

	oc = vsubstract(ray->origin, sphere->pos);
	init_quadratic(&quad, dot(ray->direction, ray->direction), \
					2.0f * dot(ray->direction, oc), \
					dot(oc, oc) - (sphere->sphere_radius * \
									sphere->sphere_radius));
	if (!solve_quadratic(&quad))
		return (false);
	if (quad.t1 > 0)
		*t = quad.t1;
	else if (quad.t2 > 0)
		*t = quad.t2;
	else
		return (false);
	return (true);
}

/**
 * intersect_cylinder - Calcula la intersección de un rayo con un cilindro.
 * @ray: Puntero al rayo que se desea trazar.
 * @cylinder: Puntero al objeto cilindro con posición, eje, y radio definidos.
 * @t: Puntero donde se almacenará la distancia al punto de intersección
 * más cercano.
 *
 * Esta función calcula si un rayo intersecta con la superficie
 * de un cilindro infinito.
 * Usa la ecuación cuadrática derivada de la proyección del rayo y
 * el eje del cilindro para determinar las soluciones. Si hay intersección,
 * almacena la distancia al punto
 * más cercano en el puntero `t` y retorna true. Si no hay
 * intersección, retorna false.
 *
 * Retorna:
 * - true si el rayo intersecta el cilindro.
 * - false si no hay intersección o las soluciones son negativas.
 */
bool	intersect_cylinder(t_ray *ray, t_obj *cylinder, float *t)
{
	t_p3		oc;
	t_p3		d_proj;
	t_p3		oc_proj;
	t_quadratic	quad;

	oc = vsubstract(ray->origin, cylinder->pos);
	d_proj = vsubstract(ray->direction, \
						scal_x_vec(dot(ray->direction, cylinder->axis), \
						cylinder->axis));
	oc_proj = vsubstract(oc, scal_x_vec(dot(oc, cylinder->axis), \
							cylinder->axis));
	init_quadratic(
		&quad,
		dot(d_proj, d_proj),
		2.0f * dot(d_proj, oc_proj),
		dot(oc_proj, oc_proj) - (cylinder->sphere_radius * \
			cylinder->sphere_radius));
	if (!solve_quadratic(&quad))
		return (false);
	if (quad.t1 > 0)
		*t = quad.t1;
	else if (quad.t2 > 0)
		*t = quad.t2;
	else
		return (false);
	return (true);
}

/**
 * intersect_plane - Calcula la intersección de un rayo con un plano.
 * @ray: Puntero al rayo que se desea trazar.
 * @plane: Puntero al objeto plano con posición y normal definidos.
 * @t: Puntero donde se almacenará la distancia al punto de
 * intersección más cercano.
 *
 * Esta función calcula si un rayo intersecta con un plano. Usa la ecuación
 * del plano para determinar el valor de t, que representa la distancia desde
 * el origen del rayo hasta el punto de intersección.
 * Si hay intersección, almacena el valor de t en el puntero proporcionado
 * y retorna true. Si el rayo es paralelo
 * al plano o la intersección está detrás del origen, retorna false.
 *
 * Retorna:
 * - true si el rayo intersecta el plano.
 * - false si no hay intersección o si el rayo es paralelo.
 */
bool	intersect_plane(t_ray *ray, t_obj *plane, float *t)
{
	float	denominator;
	float	numerator;

	denominator = dot(ray->direction, plane->axis);
	if (fabsf(denominator) < 1e-6)
		return (false);

	numerator = dot(vsubstract(plane->pos, ray->origin), plane->axis);
	*t = numerator / denominator;

	return (*t > 0);
}
