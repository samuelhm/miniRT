/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:37:51 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/01 13:08:16 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"
#include "../lib/libvector/libvct.h"

/*
	Retorna un objeto temportal para probar el render
*/
t_data	*get_test()
{
	t_data	*scene;

	scene = malloc(sizeof(t_data));
	if (!scene)
		return (NULL);
	scene->aLight = malloc(sizeof(t_aLight));
	scene->cam = malloc(sizeof(t_cam));
	scene->obj = malloc(sizeof(t_obj));
	if (!scene->aLight || !scene->cam || !scene->obj)
		return (NULL);

	scene->next = NULL;
	scene->sLight = NULL;

	scene->aLight->br = 0.2f;
	scene->aLight->rgb.r = 255;
	scene->aLight->rgb.g = 255;
	scene->aLight->rgb.b = 255;

	scene->cam->axis.x = -50.0f;
	scene->cam->axis.y = 0.0f;
	scene->cam->axis.z = -50.0f;
	scene->cam->pos.x = 0.0f;
	scene->cam->pos.y = 0.0f;
	scene->cam->pos.z = 1.0f;
	scene->cam->fov = 70;

	scene->obj = malloc(sizeof(t_obj));
	scene->obj->type = SP;
	scene->obj->pos.x = 0.0f;
	scene->obj->pos.y = 0.0f;
	scene->obj->pos.z = 20.6f;
	scene->obj->height = 12.6f;
	scene->obj->sphere_radius = 12.6f / 2.0f;
	scene->obj->color = get_acolour(0, 10, 0, 255);
	scene->obj->next = NULL;

	return (scene);
}

//Returns bidimensional RGB array to make the final image, x and y must be 0
t_rgb	**render(t_data *scene, int x, int y)
{
	t_ray		**rays;
	t_projplane	*pplane;
	t_rgb		**image;

	rays = malloc(sizeof(t_ray *) * WINDOW_WIDTH);
	for (int i = 0; i < WINDOW_WIDTH; i++)
	{
		rays[i] = malloc(sizeof(t_ray) * WINDOW_HEIGHT);
	}
	pplane = init_projection_plane(scene->cam);
	init_rays(rays, pplane, scene->cam);
	image = init_image_(WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!image)
		return (NULL);
	x = 0;
	// printf("%p\n", image[0][0]);
	t_rgb	hola;


	while (x < WINDOW_WIDTH)
	{
		y = 0;
		while (y < WINDOW_HEIGHT)
		{
			hola = trace_ray(&rays[x][y], scene);
			image[x][y].r = hola.r;
			image[x][y].g = hola.g;
			image[x][y].b = hola.b;
			y++;
		}
		x++;
	}
		printf("entra\n");


	return (image);
}
