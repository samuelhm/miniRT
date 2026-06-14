/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 14:37:48 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 17:31:49 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_rgb	color_mul(t_rgb c, double factor)
{
	t_rgb	result;

	result.r = fmin(c.r * factor, 255);
	result.g = fmin(c.g * factor, 255);
	result.b = fmin(c.b * factor, 255);
	return (result);
}

t_rgb	color_add(t_rgb c1, t_rgb c2)
{
	t_rgb	result;

	result.r = fmin(c1.r + c2.r, 255);
	result.g = fmin(c1.g + c2.g, 255);
	result.b = fmin(c1.b + c2.b, 255);
	return (result);
}

t_v3	refract(t_obj *obj, t_v3 dir, t_v3 normal)
{
	double	cosi;
	double	eta;
	double	eta2;
	double	k;

	cosi = fmax(-1, fmin(1, dot(dir, normal)));
	eta = obj->calcs.eta;
	eta2 = obj->calcs.eta2;
	if (cosi > 0)
	{
		eta = obj->calcs.eta_reverse;
		eta2 = obj->calcs.eta_reverse2;
	}
	k = 1 - eta2 * (1 - cosi * cosi);
	if (k < 0)
		return (vdefine(0, 0, 0));
	return (vadd(vmul(eta, dir), vmul(eta * cosi - sqrt(k), normal)));
}

t_v3	reflect(t_v3 dir, t_v3 normal)
{
	return (vsub(dir, vmul(2 * dot(dir, normal), normal)));
}

double	fresnel(t_obj *obj, t_v3 dir, t_v3 normal)
{
	t_fresnel	fresnel;

	fresnel.etat = obj->calcs.etat;
	fresnel.etai = obj->calcs.etai;
	fresnel.etai_etat = obj->calcs.etai_etat;
	fresnel.cosi = fabs(dot(dir, normal));
	if (dot(dir, normal) > 0)
	{
		fresnel.etat = obj->calcs.etai;
		fresnel.etai = obj->calcs.etat;
		fresnel.etai_etat = obj->calcs.etai_etat_reverse;
	}
	fresnel.sint = fresnel.etai_etat * sqrt(fmax(0.f, 1 - fresnel.cosi * \
														fresnel.cosi));
	if (fresnel.sint >= 1)
		return (1);
	fresnel.cost = sqrt(fmax(0.f, 1 - fresnel.sint * fresnel.sint));
	fresnel.rs = ((fresnel.etat * fresnel.cosi) - (fresnel.etai * \
					fresnel.cost)) / ((fresnel.etat * fresnel.cosi) \
					+ (fresnel.etai * fresnel.cost));
	fresnel.rp = ((fresnel.etai * fresnel.cosi) - (fresnel.etat * \
					fresnel.cost)) / ((fresnel.etai * fresnel.cosi) \
					+ (fresnel.etat * fresnel.cost));
	return ((fresnel.rs * fresnel.rs + fresnel.rp * fresnel.rp) / 2);
}
