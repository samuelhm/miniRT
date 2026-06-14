/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rays.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 13:34:39 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/15 15:22:11 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	init_single_rayc(t_ray *ray, t_vp *vp, t_cam *cam, double *uv)
{
	t_v3	pixel_position;

	pixel_position = vadd(vadd(vp->lower_left, vmul(uv[0], vp->horizontal)),
			vmul(uv[1], vp->vertical));
	ray->origin = cam->pos;
	ray->direction = normalize(vsub(pixel_position, cam->pos));
	ray->i_direction = normalize(vneg(ray->direction));
}

static t_ray	*init_ray_rowc(t_data *data, t_cam *cam, t_vp *vp, int y)
{
	t_ray	*row;
	int		x;
	double	uv[2];

	row = calloc(data->x, sizeof(t_ray));
	if (!row)
		return (NULL);
	x = 0;
	while (x < data->x)
	{
		uv[0] = ((double)x) / (double)(data->x - 1);
		uv[1] = 1.0f - ((double)y) / (double)(data->y - 1);
		init_single_rayc(&row[x], vp, cam, uv);
		x++;
	}
	return (row);
}

t_ray	**init_raysc(t_data *data, t_cam *cam, t_vp *vp)
{
	t_ray	**rays;
	int		y;

	rays = calloc(data->y, sizeof(t_ray *));
	if (!rays)
		return (NULL);
	y = 0;
	while (y < data->y)
	{
		rays[y] = init_ray_rowc(data, cam, vp, y);
		if (!rays[y])
		{
			free_rays(rays, y);
			return (NULL);
		}
		y++;
	}
	return (rays);
}
