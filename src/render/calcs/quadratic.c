/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:20:14 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 12:25:49 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"
#include "libvct.h"

//init Data for a quadratic equation
void	init_quadratic(t_quadratic *quad, double a, double b, double c)
{
	if (fabs(a) < EPSILON)
	{
		quad->a = 0.0f;
		quad->discriminant = -1.0f;
		quad->t1 = INFINITY;
		quad->t2 = INFINITY;
		return ;
	}
	quad->a = a;
	quad->b = b;
	quad->c = c;
	quad->discriminant = b * b - 4 * a * c;
	quad->t1 = INFINITY;
	quad->t2 = INFINITY;
}

bool	solve_quadratic(t_quadratic *quad)
{
	double	sqrt_discriminant;
	double	ax2;

	ax2 = 2.0f * quad->a;
	if (quad->discriminant < 0 || fabs(quad->a) < EPSILON)
		return (false);
	sqrt_discriminant = sqrt(quad->discriminant);
	quad->t1 = (-quad->b - sqrt_discriminant) / ax2;
	quad->t2 = (-quad->b + sqrt_discriminant) / ax2;
	if (quad->t1 > quad->t2)
		swap(&quad->t1, &quad->t2);
	if (quad->t1 < EPSILON)
	{
		quad->t1 = quad->t2;
		quad->t2 = INFINITY;
		if (quad->t1 < EPSILON)
			return (false);
	}
	return (true);
}

bool	solve_quadratic2(t_quadratic *quad)
{
	double	sqrt_discriminant;
	double	ax2;

	ax2 = 2.0f * quad->a;
	if (quad->discriminant < 0 || fabs(quad->a) < EPSILON)
		return (false);
	sqrt_discriminant = sqrt(quad->discriminant);
	quad->t1 = (-quad->b - sqrt_discriminant) / ax2;
	quad->t2 = (-quad->b + sqrt_discriminant) / ax2;
	if (quad->t1 > EPSILON || quad->t2 > EPSILON)
		return (true);
	return (false);
}
