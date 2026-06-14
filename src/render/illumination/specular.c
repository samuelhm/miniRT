/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:41:20 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/11 12:36:50 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	specular_light(t_rgb *color, t_data *data, t_ray *ray, int shin)
{
	double		intensity;
	t_v3		reflection;
	t_slight	*sl;

	sl = data->s_light;
	while (sl)
	{
		reflection = vrefl(vneg(normalize(vsub(sl->pos, ray->point))), \
					ray->normal);
		intensity = pow(fmax(dot(reflection, ray->i_direction), 0.0f), shin);
		color->r = fmin(color->r + sl->rgb.r * intensity, 255);
		color->g = fmin(color->g + sl->rgb.g * intensity, 255);
		color->b = fmin(color->b + sl->rgb.b * intensity, 255);
		sl = sl->next;
	}
}
