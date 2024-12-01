/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:55:24 by erigonza          #+#    #+#             */
/*   Updated: 2024/12/01 13:07:57 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# define RESET "\033[0m"
# define BOLD "\033[1m"
# define RED_BK "\033[41m" // background
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# define M_PI 3.14159265358979323846
# define PPLANEDISTANCE 1.0f

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/inc/libft.h"
# include "../lib/libvector/libvct.h"
# include "render.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

# define WINDOW_WIDTH 800
# define WINDOW_HEIGHT 600

# define WIDTH 500
# define HEIGHT 500
# define SP 0
# define PL 1
# define CY 2

typedef unsigned char	t_uchar;
// t_rgb[800][600] image;

typedef struct s_rgb
{
	unsigned char			r;
	unsigned char			g;
	unsigned char			b;
}					t_rgb;

typedef struct s_cam
{
	t_v3			pos;
	int				fov;
	t_v3			axis; // orientation
}					t_cam;

typedef struct s_aLight
{
	float			br; // brightness
	t_rgb			rgb;
}					t_aLight;

typedef struct s_sLight
{
	t_v3			pos;
	float			br; // brightness
	t_rgb			rgb;
	struct s_light	*next;
}					t_sLight;

typedef struct s_obj
{
	uint32_t		color;
	int				i;			// just to use it on the parser
	char			type;		// in case I want to separate the parser and the exec
	t_v3			pos;		// cp pl cy
	t_v3			axis;		// pl cy | orientation
	t_rgb			rgb;		// sp pl cy
	float			size;		// sp radius | cy diameter
	float			height;		// cy
//
	float			sphere_radius; // size
	t_v3			ray_start; // Camera position
	t_v3			sphere_center;
	struct s_obj	*next;
}					t_obj;

typedef struct s_data
{
	t_aLight		*aLight;
	t_cam			*cam;
	t_sLight		*sLight;
	t_obj			*obj;
	struct s_data	*next;
}					t_data;



//		utils
int					er(char *s, char *av);
t_obj				*newObj(t_obj *obj);
float				ft_atof(char *str, int i);
float				ft_atof_normi(char *str, int i);

//		parsing
t_obj				*parse(t_data *data, t_obj *obj, char **av, int fd);
void				correct_file(char *name);

//		parseACLUtils
void				createACL(t_data *data, char *str, int type);

//		parseUtils
int					checkObj(t_data *data, char *str);
int					sumParse(char *str, int i, int flag, int j);
char				*floatsParse(t_obj *obj, char *str, int i, int flag);
int					ft_atoiParse(char *str, int i);
t_rgb				colorsParse(char *str);
//		lib
t_v3				subtract(t_v3 a, t_v3 b);
float				dot(t_v3 a, t_v3 b);
t_v3				vDefine(float x, float y, float z);

//		mlx
void				draw_pixel(mlx_image_t *img, int x, int y, uint32_t color);
void				my_keyhook(mlx_key_data_t keydata, void *param);

//		sphere
void				ft_sphere(t_obj *sp, t_sLight *light, mlx_image_t *img);
float				sphere_ray_intersect(t_v3 ray_start, t_v3 ray_direction,
						t_v3 sphere_center, float sphere_radius);
//		ilumination
int					is_in_shadow(t_obj *sp, t_v3 point, t_sLight *light);
uint32_t			new_light(t_sLight *l, t_obj *sp, t_v3 iPoint);


typedef struct s_projection_plane
{
	float	width;
	float	height;
	float	distance;
	t_v3	forward;
	t_v3	right;
	t_v3	up;
}		t_projplane;

typedef struct s_ray
{
	t_v3	origin;
	t_v3	direction;
}				t_ray;

typedef struct s_quadratic
{
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float	t1; //inteseccion de entrada
	float	t2; //interseccion de salida
}			t_quadratic;


//render
uint32_t	get_acolour(t_uchar alpha, t_uchar r, t_uchar g, t_uchar b);
t_projplane	*init_projection_plane(t_cam *cam);
void		init_rays(t_ray **rays, t_projplane *pplane, t_cam *cam);
void		init_quadratic(t_quadratic *quad, float a, float b, float c);
bool		solve_quadratic(t_quadratic *quad);
t_rgb		trace_ray(t_ray *ray, t_data *scene);
t_rgb		apply_ambient_light(t_rgb obj_color, t_aLight *ambient_light);
t_rgb		**render(t_data *scene, int x, int y);
t_data		*get_test();
bool	intersect_sphere(t_ray *ray, t_obj *sphere, float *t);
t_rgb	**init_image_(int width, int height);



#endif
