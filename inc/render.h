/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 11:25:17 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/23 10:41:48 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "rt.h"

# define PPLANEDISTANCE 1.0f
# define BLACK 0xFF000000

typedef unsigned char	t_uchar;
constexpr double	EPSILON = 1e-4;
# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

// Materials
# define SOLID_S 12

// Color Weight
# define G_WEIGHT 0.7
# define L_WEIGHT 0.3

constexpr int	MAX_DEPTH = 5;

# define FLT_MAX 3.40282347e+38F

typedef struct s_viewport
{
	double				viewport_width;
	double				viewport_height;
	t_v3				origin;
	t_v3				horizontal;
	t_v3				vertical;
	t_v3				lower_left;
}						t_vp;

typedef struct s_ray
{
	t_v3				origin;
	t_v3				direction;
	t_v3				i_direction;
	t_v3				normal;
	t_v3				point;
}						t_ray;

typedef struct s_direct
{
	t_slight			*slight;
	t_ray				shadow_ray;
	t_rgb				specular_color;
	double				intensity;
	t_v3				vsub_pos_point;
	t_v3				reflect_dir;
	t_v3				view_dir;
	double				spec_intensity;
	t_rgb				*color;
}						t_direct;

typedef struct s_quadratic
{
	double				a;
	double				b;
	double				c;
	double				discriminant;
	double				t1;
	double				t2;
}						t_quadratic;

//		calcs
t_obj					*find_closest(t_data *restrict data, t_ray *restrict ray,
							double *restrict t_min);
t_rgb					diffuse_ray(t_ray *ray, t_obj *closest, t_data *data,
							int depth);
t_rgb					compute_direct_light(t_obj *obj, t_data *data,
							t_ray *ray, t_rgb color);
t_rgb					path_trace(t_ray *ray, t_data *data, int depth);
uint32_t				trace_ray(t_ray ray, t_data *data);
uint32_t				trace_ray_dbg(t_ray ray, t_data *data, int px, int py);

//		calcs_utils
t_v3					random_in_hemisphere(t_v3 normal);
t_rgb					apply_self_emission(t_obj *obj, t_rgb base_color);
void					compute_emissive_light(t_obj *emitter, t_ray *ray,
							t_rgb *color, t_data *data);
void					iter_lights(t_data *data, t_obj *obj, t_ray *ray,
							t_direct d);
bool					pt_checks(t_obj *closest, t_ray *ray, t_rgb *base_color);

//		calcs_utils2
uint32_t				texture_weight(t_rgb c1, t_rgb c2);
t_rgb					path_trace_type(t_ray *ray, t_obj *closest, \
							t_data *data, int depth);

//		materials
t_rgb					glass_ray(t_ray *ray, t_obj *closest, t_data *data,
							int depth);
t_rgb					mirror_ray(t_ray *ray, t_obj *closest, t_data *data,
							int depth);
t_v3					perturb_vector(t_v3 direction, double roughness,
							t_v3 normal);
t_rgb					metallic_ray(t_ray *ray, t_obj *closest, t_data *data,
							int depth);

//		materials_utils
t_rgb					color_mul(t_rgb c, double factor);
t_rgb					color_add(t_rgb c1, t_rgb c2);
t_v3					refract(t_obj *obj, t_v3 dir, t_v3 normal);
t_v3					reflect(t_v3 dir, t_v3 normal);
double					fresnel(t_obj *obj, t_v3 dir, t_v3 normal);

//		quadratic
void					init_quadratic(t_quadratic *quad, double a, double b,
							double c);
bool					solve_quadratic(t_quadratic *quad);
bool					solve_quadratic2(t_quadratic *quad);

//		color
t_rgb					rgbdefine(t_uchar r, t_uchar g, t_uchar b);
uint32_t				get_acolour(t_uchar alpha, t_uchar r, t_uchar g,
							t_uchar b);
uint32_t				get_colour(t_rgb color);
void					unpack(uint32_t color, t_rgb *s);
uint32_t				average(uint32_t c1, uint32_t c2, double w);

//		extras
t_rgb					checkerboard_color(t_obj *obj, t_v3 point);

//		init_general
void					init_all(t_data *data);
void					init_data(t_data **data);

//		init_image
void					init_mlx(t_data *data);
uint32_t				*init_image_(t_data *data);

//		init_obj
void					init_obj(t_data *data);
void					init_light(t_data *data);
void					set_calcs(t_data *data, t_obj *obj);
void					make_cone_cap(t_obj *cone, t_data *data);
void					make_caps(t_data *data, t_obj *obj);

//		init_obj_utils
void					init_obj_normi(t_data *data, t_obj *obj);
void					init_set_prev(t_data *data);

//		init_sides
void					init_sides(t_data *data, t_obj *obj);
void					set_sides_xy(t_obj **sides, t_obj *cube);
void					set_box_local_axes(t_obj *box, t_v3 raw_z);
void					set_some_sides(t_obj **side, t_obj *cube,
							t_v3 half_size);
void					create_side(t_obj **side, t_obj *cube);

//		init_sides_utils
t_frame					set_frame(t_v3 axis);

//		init_rays
t_ray					**init_raysc(t_data *data, t_cam *cam, t_vp *vp);
void					init_single_ray(t_ray *ray, t_vp *vp, t_cam *camera,
							double *uv);
t_ray					*init_ray_row(t_data *data, t_cam *camera, t_vp *vp,
							int y);
t_ray					**init_rays(t_data *data, t_cam *camera, t_vp *vp);

//		init_rays_utils
void					free_rays(t_ray **rays, int rows);

//		init_materials
void					init_gl(t_obj *obj);
void					init_mt(t_obj *obj);
void					init_mr(t_obj *obj);

//		cylinder
void					set_cy_axis(t_quadratic *quad, t_obj *cy, t_ray *ray);
double					set_ray_t(t_ray *ray, t_obj *cy, double *t,
							t_quadratic quad);
bool					hit_cy(t_ray *restrict ray, t_obj *restrict cy,
							double *restrict t);
//		caps
bool					hit_cap(t_data *restrict data, t_ray *restrict ray,
							t_obj *restrict cap, double *restrict t);

//		cone
bool					hit_cone(t_ray *restrict ray, t_obj *restrict cone,
							double *restrict t);

//		cube
bool					create_cube(t_data *data, t_obj *cube);

//		illumination
t_rgb					apply_al(t_rgb obj_color, t_alight *a_light);
void					difuse_light(t_rgb *color, t_slight *slight, t_obj *obj,
							double inty);
bool					data_shadow(t_data *data, t_ray *shadow_ray,
							double max_dist, t_obj *self);
t_rgb					phong(t_data *data, t_ray *ray, t_obj *obj);

//		intersections
bool					calc_quad_sphere(t_obj *sphere, t_ray ray,
							t_quadratic *quad);
bool					hit_sp(t_ray *restrict ray, t_obj *restrict sphere,
							double *restrict t);
bool					hit_pl(t_data *restrict data, t_ray *restrict ray,
							t_obj *restrict plane, double *restrict t);

//		viewport
t_v3					calculate_up(t_v3 forward, t_v3 right);
t_v3					calculate_right(t_v3 forward);
t_vp					*init_viewport(t_cam *camera, int width, int height);

//		uv_map (texture)
t_rgb					texture_color(t_obj *obj, t_v2 uv);
t_v2					calculate_uv(t_v3 point, t_obj *obj);

//		uv_obj (texture)
t_v2					sphere_uv(t_v3 point, t_obj *sphere);
t_v2					side_uv(t_v3 point, t_obj *plane);
t_v2					plane_uv(t_v3 point, t_obj *plane);
t_v2					cylinder_uv(t_v3 point, t_obj *cyl);
t_v2					cone_uv(t_v3 point, t_obj *cone);

//		bump map sphere
void					get_sphere_normal(t_obj *sphere, t_ray *ray);

//		bump map plane
void					get_plane_normal(t_obj *plane, t_v3 hit_point,
							t_ray *ray);

//		three_renders
void					render_one(void *param);
void					render_fast(void *param);
void					update_render(void *param);

//		render
uint32_t				*render(t_data *data, int mode);
uint32_t				*console_render(t_data *data);
void					c_render(t_data *data, t_vp *vp, uint32_t *image);
t_ray					generate_ray(t_data *data, t_vp *vp, t_cam *cam, int x,
							int y, int mode);
void					*cprocess_rows(void *arg);
uint32_t				trace_fast(t_ray ray, t_data *data);

//		free
void					free_rays_all(t_data *data, t_ray **rays);
void					free_render(t_data *data, t_vp *vp, t_ray **rays);
void					free_image_all(t_data *data, uint32_t *image);
void					free_rays(t_ray **rays, int rows);

//		PATH_TRACER  (calcs)
t_rgb					path_trace(t_ray *ray, t_data *data, int depth);
t_v3					random_in_hemisphere(t_v3 normal);
double					*generate_uv(int x, int y);
uint32_t				*average_samples(t_data *data, uint32_t *sample1,
							uint32_t *sample2, double w);

#endif
