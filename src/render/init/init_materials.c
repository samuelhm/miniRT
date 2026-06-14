/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_materials.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:34:20 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 10:40:41 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_gl(t_obj *obj)
{
	obj->material.reflectivity = 0.1f;
	obj->material.transmittance = 0.9f;
	obj->material.roughness = 0.0f;
	obj->material.absorption = 0.1f;
	obj->material.specularity = 0.9f;
	obj->material.shininess = 150;
}

void	init_mt(t_obj *obj)
{
	obj->material.reflectivity = 0.4f;
	obj->material.transmittance = 0.0f;
	obj->material.roughness = 1.0f;
	obj->material.absorption = 1.0f;
	obj->material.specularity = 0.9f;
	obj->material.shininess = 170;
}

void	init_mr(t_obj *obj)
{
	obj->a_rgb = rgbdefine(0, 0, 0);
	obj->material.reflectivity = 1.0f;
	obj->material.transmittance = 0.0f;
	obj->material.roughness = 0.0f;
	obj->material.absorption = 0.0f;
	obj->material.specularity = 1.0f;
	obj->material.shininess = 500;
}
