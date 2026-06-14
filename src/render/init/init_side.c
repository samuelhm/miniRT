/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_side.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:39:47 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/18 17:21:42 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	set_sides_xy(t_obj **sides, t_obj *cube)
{
	if (!cube || !sides)
		return ;
	sides[0]->size = cube->cube_size.y;
	sides[0]->height = cube->cube_size.z;
	sides[1]->size = cube->cube_size.y;
	sides[1]->height = cube->cube_size.z;
	sides[2]->size = cube->cube_size.x;
	sides[2]->height = cube->cube_size.z;
	sides[3]->size = cube->cube_size.x;
	sides[3]->height = cube->cube_size.z;
	sides[4]->size = cube->cube_size.x;
	sides[4]->height = cube->cube_size.y;
	sides[5]->size = cube->cube_size.x;
	sides[5]->height = cube->cube_size.y;
}

void	set_box_local_axes(t_obj *box, t_v3 raw_z)
{
	t_v3	forward;
	t_v3	aux;
	t_v3	raw_up;
	double	len;

	raw_up = vdefine(0, 1, 0);
	forward = normalize(raw_z);
	if (fabs(dot(forward, raw_up)) > 0.99)
	{
		raw_up.x = 1;
		raw_up.y = 0;
		raw_up.z = 0;
	}
	raw_up = normalize(raw_up);
	aux = cross(raw_up, forward);
	len = vlength(aux);
	if (len < 1e-6)
		aux = vdefine(1, 0, 0);
	aux = normalize(aux);
	box->axis_z = forward;
	box->axis_x = aux;
	box->axis_y = cross(box->axis_z, box->axis_x);
	box->axis_y = normalize(box->axis_y);
}

void	set_some_sides(t_obj **side, t_obj *cube, t_v3 half_size)
{
	side[0]->axis = cube->axis_x;
	side[0]->pos = vadd(cube->pos, vmul(half_size.x, cube->axis_x));
	side[0]->right = cube->axis_y;
	side[0]->up = cube->axis_z;
	side[0]->face = 1;
	side[1]->axis = vmul(-1.0, cube->axis_x);
	side[1]->pos = vadd(cube->pos, vmul(-half_size.x, cube->axis_x));
	side[1]->right = cube->axis_y;
	side[1]->up = cube->axis_z;
	side[1]->face = 1;
	side[2]->axis = cube->axis_y;
	side[2]->pos = vadd(cube->pos, vmul(half_size.y, cube->axis_y));
	side[2]->right = cube->axis_x;
	side[2]->up = cube->axis_z;
	side[2]->face = 2;
	side[3]->axis = vmul(-1.0, cube->axis_y);
	side[3]->pos = vadd(cube->pos, vmul(-half_size.y, cube->axis_y));
	side[3]->right = cube->axis_x;
	side[3]->up = cube->axis_z;
	side[3]->face = 2;
}

void	create_side(t_obj **side, t_obj *cube)
{
	t_v3	half_size;

	half_size = vmul(0.5, cube->cube_size);
	set_some_sides(side, cube, half_size);
	side[4]->axis = cube->axis_z;
	side[4]->pos = vadd(cube->pos, vmul(half_size.z, cube->axis_z));
	side[4]->right = cube->axis_x;
	side[4]->up = cube->axis_y;
	side[4]->face = 3;
	side[5]->axis = vmul(-1.0, cube->axis_z);
	side[5]->pos = vadd(cube->pos, vmul(-half_size.z, cube->axis_z));
	side[5]->right = cube->axis_x;
	side[5]->up = cube->axis_y;
	side[5]->face = 3;
	set_sides_xy(side, cube);
}

void	init_sides(t_data *data, t_obj *obj)
{
	t_obj	**side;
	int		i;

	i = -1;
	set_box_local_axes(obj, obj->axis);
	side = calloc(6, sizeof(t_obj *));
	if (!side)
		exit(er(obj->data, "error: init_sides: malloc", NULL));
	while (++i < 6)
	{
		side[i] = calloc(1, sizeof(t_obj));
		memcpy(side[i], obj, sizeof(t_obj));
		side[i]->type = SIDE;
		side[i]->calcs = obj->calcs;
		side[i]->next = NULL;
		side[i]->material.emision = 0;
		objadd_back(&data->obj, side[i]);
	}
	create_side(side, obj);
	free(side);
}
