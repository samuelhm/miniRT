/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 12:37:59 by shurtado          #+#    #+#             */
/*   Updated: 2024/12/01 13:08:02 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/render.h"
#include "../lib/libvector/libvct.h"
#include "../inc/miniRT.h"

/**
 * init_projection_plane - Inicializa el plano de proyección.
 * @cam: Puntero a la cámara que define la posición, orientación y FOV.
 *
 * Calcula las dimensiones y los vectores ortogonales (forward, right, up)
 * del plano de proyección basado en la cámara. Estos valores son
 * fundamentales para generar rayos que atraviesan cada píxel de la ventana.
 *
 * Retorna una estructura t_projplane inicializada.
 */
t_projplane	*init_projection_plane(t_cam *cam)
{
	t_projplane	*pplane;
	float		fov_radius;
	t_v3		up_guide;

	pplane = malloc(sizeof(t_projplane));
	pplane->distance = PPLANEDISTANCE;
	fov_radius = cam->fov * M_PI / 180.0f;
	pplane->width = 2.0f * tan(fov_radius / 2.0f) * pplane->distance;
	pplane->height = pplane->width * ((float)WINDOW_HEIGHT / (float)WINDOW_WIDTH);
	pplane->forward = normalize(cam->axis);

	up_guide.x = 0.0f;
	up_guide.y = 1.0f;
	up_guide.z = 0.0f;

	if (fabs(dot(pplane->forward, up_guide)) > 0.999f)
	{
		up_guide.x = 1.0f;
		up_guide.y = 0.0f;
	}
	pplane->right = normalize(cross(up_guide, pplane->forward));

	pplane->up = cross(pplane->forward, pplane->right);
	return (pplane);
}

/**
 * calculate_ray - Calcula un rayo para un píxel específico.
 * @x: Coordenada horizontal del píxel en la ventana.
 * @y: Coordenada vertical del píxel en la ventana.
 * @pplane: Puntero al plano de proyección previamente inicializado.
 * @cam: Puntero a la cámara que define el origen de los rayos.
 *
 * Genera un rayo con origen en la posición de la cámara y dirección hacia
 * un punto en el plano de proyección correspondiente al píxel (x, y).
 *
 * Retorna t_ray con la posición de origen y la dirección normalizada.
 */
t_ray	calculate_ray(int x, int y, t_projplane *pplane, t_cam *cam)
{
	t_ray	ray;
	float	u;
	float	v;
	t_p3	point_on_plane;

	u = ((float)x + 0.5f) / WINDOW_WIDTH - 0.5f;
	v = 0.5f - ((float)y + 0.5f) / WINDOW_HEIGHT;
	u *= pplane->width;
	v *= pplane->height;

	point_on_plane = vadd(vadd(scal_x_vec(pplane->distance, pplane->forward), \
			scal_x_vec(u, pplane->right)), scal_x_vec(v, pplane->up));
	ray.origin = cam->pos;
	ray.direction = normalize(vsubstract(point_on_plane, cam->pos));

	return (ray);
}

/**
 * init_rays - Inicializa todos los rayos de la ventana.
 * @rays: Matriz bidimensional donde se almacenarán los rayos.
 * @pplane: Puntero al plano de proyección previamente inicializado.
 * @cam: Puntero a la cámara que define el origen de los rayos.
 *
 * Recorre todos los píxeles de la ventana y genera un rayo para cada uno
 * utilizando el plano de proyección y la cámara. Los rayos son almacenados
 * en la matriz bidimensional proporcionada.
 */
void	init_rays(t_ray **rays, t_projplane *pplane, t_cam *cam)
{
	int		x;
	int		y;

	y = 0;
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			rays[x][y] = calculate_ray(x, y, pplane, cam);
			x++;
		}
		y++;
	}
}

//init Data for a quadratic equation
void	init_quadratic(t_quadratic *quad, float a, float b, float c)
{
	quad->a = a;
	quad->b = b;
	quad->c = c;
	quad->discriminant = b * b - 4 * a * c;
	quad->t1 = INFINITY;
	quad->t2 = INFINITY;
}

//Memmory for the final image
t_rgb	**init_image_(int width, int height)
{
	t_rgb	**image;
	int		x;

	image = malloc(width * sizeof(t_rgb *));
	if (!image)
		return (NULL);
	x = 0;
	while (x < width)
	{
		image[x] = malloc(height * sizeof(t_rgb));
		if (!image[x])
		{
			while (--x >= 0)
				free(image[x]);
			free(image);
			return (NULL);
		}
		x++;
	}
	return (image);
}
