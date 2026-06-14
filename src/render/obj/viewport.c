/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viewport.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 17:40:08 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/13 11:26:32 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#include "render.h"

t_v3	calculate_up(t_v3 forward, t_v3 rgt)
{
	return (normalize(cross(forward, rgt)));
}

t_v3	calculate_right(t_v3 forward)
{
	t_v3	arbitrary;

	if (fabs(forward.y) > 0.999)
		arbitrary = (t_v3){1, 0, 0};
	else
		arbitrary = (t_v3){0, 1, 0};
	return (normalize(cross(arbitrary, forward)));
}

t_vp	*init_viewport(t_cam *camera, int width, int height)
{
	t_vp	*viewport;
	double	aspect_ratio;

	camera->axis = normalize(camera->axis);
	viewport = calloc(1, sizeof(t_vp));
	if (!viewport)
		return (NULL);
	aspect_ratio = (double)width / (double)height;
	viewport->viewport_width = 2.0 * tan((camera->fov * M_PI / 180.0) / 2.0);
	viewport->viewport_height = viewport->viewport_width / aspect_ratio;
	camera->frame.forward = camera->axis;
	camera->frame.right = calculate_right(camera->frame.forward);
	camera->frame.up = calculate_up(camera->frame.forward, camera->frame.right);
	viewport->origin = camera->pos;
	viewport->horizontal = vmul(viewport->viewport_width, camera->frame.right);
	viewport->vertical = vmul(viewport->viewport_height, camera->frame.up);
	viewport->lower_left = vadd(vsub(vsub(viewport->origin, \
										scalar_div(viewport->horizontal, 2)), \
										scalar_div(viewport->vertical, 2)), \
										camera->frame.forward);
	return (viewport);
}
