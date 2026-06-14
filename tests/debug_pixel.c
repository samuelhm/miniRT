#include "rt.h"
#include <stdio.h>

_Thread_local int	g_dbg = 0;
_Thread_local int	g_dbg_depth = 0;

static void	dbg_indent(void)
{
	for (int i = 0; i < g_dbg_depth; i++)
		fprintf(stderr, "  ");
}

static const char	*type_str(int t)
{
	switch (t) {
	case 0: return ("SP");
	case 1: return ("PL");
	case 2: return ("CY");
	case 3: return ("CAP");
	case 6: return ("CO");
	case 7: return ("CU");
	case 8: return ("SIDE");
	default: return ("??");
	}
}

static const char	*mat_str(int m)
{
	switch (m) {
	case -1: return ("DIFFUSE");
	case 0: return ("METALLIC");
	case 1: return ("GLASS");
	case 2: return ("MIRROR");
	case 3: return ("CB");
	case 4: return ("EM");
	default: return ("??");
	}
}

static void	ray_dump(const char *label, t_ray *r)
{
	dbg_indent();
	fprintf(stderr, "%s o=(%.4f,%.4f,%.4f) d=(%.6f,%.6f,%.6f) norm=(%.6f,%.6f,%.6f) pt=(%.4f,%.4f,%.4f)\n",
		label,
		r->origin.x, r->origin.y, r->origin.z,
		r->direction.x, r->direction.y, r->direction.z,
		r->normal.x, r->normal.y, r->normal.z,
		r->point.x, r->point.y, r->point.z);
}

static void	rgb_dump(const char *label, t_rgb c)
{
	dbg_indent();
	fprintf(stderr, "%s rgb(%d,%d,%d)\n", label, c.r, c.g, c.b);
}

static t_rgb	path_trace_dbg(t_ray *ray, t_data *data, int depth);

static t_rgb	diffuse_ray_dbg(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	new_ray;
	t_rgb	trace_color;

	dbg_indent();
	fprintf(stderr, "DIFFUSE_BOUNCE reflectivity=%.3f\n",
		closest->material.reflectivity);
	new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	new_ray.direction = random_in_hemisphere(ray->normal);
	g_dbg_depth++;
	trace_color = path_trace_dbg(&new_ray, data, depth - 1);
	g_dbg_depth--;
	trace_color = color_mul(trace_color, closest->material.reflectivity);
	dbg_indent();
	fprintf(stderr, "DIFFUSE_RESULT ");
	rgb_dump("", trace_color);
	return (trace_color);
}

static t_rgb	metallic_ray_dbg(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	new_ray;
	t_v3	perturbed_direction;

	dbg_indent();
	fprintf(stderr, "METALLIC_BOUNCE reflectivity=%.3f roughness=%.3f\n",
		closest->material.reflectivity, closest->material.roughness);
	new_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	new_ray.direction = reflect(ray->direction, ray->normal);
	if (closest->material.roughness > 0)
	{
		perturbed_direction = perturb_vector(new_ray.direction,
					closest->material.roughness, ray->normal);
		new_ray.direction = perturbed_direction;
	}
	g_dbg_depth++;
	t_rgb res = path_trace_dbg(&new_ray, data, depth - 1);
	g_dbg_depth--;
	res = color_mul(res, closest->material.reflectivity);
	dbg_indent();
	fprintf(stderr, "METALLIC_RESULT ");
	rgb_dump("", res);
	return (res);
}

static t_rgb	mirror_ray_dbg(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	new_ray;
	t_v3	normal;

	dbg_indent();
	fprintf(stderr, "MIRROR_BOUNCE reflectivity=%.3f\n",
		closest->material.reflectivity);
	normal = ray->normal;
	new_ray.origin = vadd(ray->point, vmul(EPSILON, normal));
	new_ray.direction = reflect(ray->direction, normal);
	g_dbg_depth++;
	t_rgb pt = path_trace_dbg(&new_ray, data, depth - 1);
	g_dbg_depth--;
	t_rgb res = color_mul(pt, closest->material.reflectivity);
	dbg_indent();
	fprintf(stderr, "MIRROR_RESULT ");
	rgb_dump("", res);
	return (res);
}

static t_rgb	glass_ray_dbg(t_ray *ray, t_obj *closest, t_data *data, int depth)
{
	t_ray	reflected_ray;
	t_ray	refracted_ray;
	t_rgb	reflected_color;
	t_rgb	refracted_color;

	dbg_indent();
	closest->calcs.kr = fresnel(closest, ray->direction, ray->normal);
	fprintf(stderr, "GLASS_BOUNCE kr=%.3f\n", closest->calcs.kr);
	refracted_ray.origin = vadd(ray->point, vmul(-EPSILON, ray->normal));
	refracted_ray.direction = refract(closest, ray->direction, ray->normal);
	reflected_ray.origin = vadd(ray->point, vmul(EPSILON, ray->normal));
	reflected_ray.direction = reflect(ray->direction, ray->normal);
	g_dbg_depth++;
	reflected_color = path_trace_dbg(&reflected_ray, data, depth - 1);
	refracted_color = path_trace_dbg(&refracted_ray, data, depth - 1);
	g_dbg_depth--;
	t_rgb res = color_add(color_mul(reflected_color, closest->calcs.kr),
				color_mul(refracted_color, 1.0f - closest->calcs.kr));
	dbg_indent();
	fprintf(stderr, "GLASS_RESULT ");
	rgb_dump("", res);
	return (res);
}

static t_rgb	path_trace_type_dbg(t_ray *ray, t_obj *closest, t_data *data,
		int depth)
{
	t_rgb	res;

	res = rgbdefine(0, 0, 0);
	dbg_indent();
	fprintf(stderr, "BOUNCE type=%s\n", mat_str(closest->material.m_type));
	if (closest->material.m_type == MAT_DEFAULT)
		res = diffuse_ray_dbg(ray, closest, data, depth);
	else if (closest->material.m_type == MT)
		res = metallic_ray_dbg(ray, closest, data, depth);
	else if (closest->material.m_type == MR)
		res = mirror_ray_dbg(ray, closest, data, depth);
	else if (closest->material.m_type == GL)
		res = glass_ray_dbg(ray, closest, data, depth);
	return (res);
}

static t_rgb	path_trace_dbg(t_ray *ray, t_data *data, int depth)
{
	t_obj	*closest;
	t_rgb	base_color;
	t_rgb	direct_light;
	t_rgb	indirect_light;
	t_rgb	result;
	double	t;

	dbg_indent();
	fprintf(stderr, "=== path_trace depth=%d ===\n", depth);
	ray_dump("ray", ray);

	if (depth <= 0)
	{
		dbg_indent();
		fprintf(stderr, "MAX_DEPTH reached -> BLACK\n");
		return (rgbdefine(0, 0, 0));
	}

	t = INFINITY;
	closest = find_closest(data, ray, &t);
	if (closest && closest->material.texture)
	{
		dbg_indent();
		fprintf(stderr, "TEXTURE hit -> early return\n");
		return (texture_color(closest,
			calculate_uv(ray->point, closest)));
	}
	if (pt_checks(closest, ray, &base_color))
	{
		dbg_indent();
		fprintf(stderr, "MISS (no obj) -> BLACK\n");
		return (rgbdefine(0, 0, 0));
	}
	dbg_indent();
	fprintf(stderr, "HIT type=%s mat=%s pos=(%.2f,%.2f,%.2f) rgb=(%d,%d,%d) a_rgb=(%d,%d,%d)\n",
		type_str(closest->type), mat_str(closest->material.m_type),
		closest->pos.x, closest->pos.y, closest->pos.z,
		closest->rgb.r, closest->rgb.g, closest->rgb.b,
		closest->a_rgb.r, closest->a_rgb.g, closest->a_rgb.b);

	rgb_dump("base_color", base_color);

	direct_light = compute_direct_light(closest, data, ray, rgbdefine(0, 0, 0));
	rgb_dump("direct_light", direct_light);

	if (closest->material.m_type == EM)
		base_color = apply_self_emission(closest, base_color);

	g_dbg_depth++;
	indirect_light = path_trace_type_dbg(ray, closest, data, depth);
	g_dbg_depth--;

	result = color_add(color_add(base_color, direct_light), indirect_light);
	dbg_indent();
	fprintf(stderr, "=== path_trace depth=%d RESULT ", depth);
	rgb_dump("", result);
	return (result);
}

static void	render_one_pixel(t_data *data, int px, int py)
{
	t_vp	*vp;
	t_cam	*cam;
	t_ray	ray;

	cam = data->cam;
	data->x = 1920;
	data->y = 1080;
	vp = init_viewport(cam, data->x, data->y);
	if (!vp) { fprintf(stderr, "viewport failed\n"); return; }

	g_dbg = 1;
	g_dbg_depth = 0;

	fprintf(stderr, "=== PIXEL (%d,%d) ===\n", px, py);
	fprintf(stderr, "cam pos=(%.2f,%.2f,%.2f) axis=(%.3f,%.3f,%.3f) fov=%d\n",
		cam->pos.x, cam->pos.y, cam->pos.z,
		cam->axis.x, cam->axis.y, cam->axis.z, cam->fov);
	fprintf(stderr, "vp lower_left=(%.4f,%.4f,%.4f) H=(%.4f,%.4f,%.4f) V=(%.4f,%.4f,%.4f)\n",
		vp->lower_left.x, vp->lower_left.y, vp->lower_left.z,
		vp->horizontal.x, vp->horizontal.y, vp->horizontal.z,
		vp->vertical.x, vp->vertical.y, vp->vertical.z);

	tls_rng_seed(42);
	ray = generate_ray(data, vp, cam, px, py, 1);
	ray_dump("CAMERA_RAY", &ray);

	fprintf(stderr, "\n--- PRIMARY HIT ---\n");
	double t_min = INFINITY;
	t_obj *hit = find_closest(data, &ray, &t_min);
	if (!hit)
	{
		fprintf(stderr, "PRIMARY MISS! -> BLACK\n");
		fprintf(stderr, "Checking objects manually:\n");
		t_obj *obj = data->obj;
		while (obj)
		{
			double t = INFINITY;
			bool found = false;
			if (obj->type == SP) found = hit_sp(&ray, obj, &t);
			else if (obj->type == PL || obj->type == SIDE) found = hit_pl(data, &ray, obj, &t);
			else if (obj->type == CY) found = hit_cy(&ray, obj, &t);
			else if (obj->type == CAP) found = hit_cap(data, &ray, obj, &t);
			else if (obj->type == CO) found = hit_cone(&ray, obj, &t);
			if (found && t > 0)
				fprintf(stderr, "  MANUAL HIT type=%s i=%d pos=(%.2f,%.2f,%.2f) t=%.4f\n",
					type_str(obj->type), obj->i,
					obj->pos.x, obj->pos.y, obj->pos.z, t);
			obj = obj->next;
		}
	}
	else
	{
		fprintf(stderr, "PRIMARY HIT type=%s t=%.2f\n", type_str(hit->type), t_min);
		fprintf(stderr, "\n--- PATH TRACE ---\n");
		t_rgb final_color = path_trace_dbg(&ray, data, MAX_DEPTH);
		uint32_t packed = get_colour(final_color);
		fprintf(stderr, "\n=== FINAL COLOR rgb(%d,%d,%d) packed=0x%08X ===\n",
			final_color.r, final_color.g, final_color.b, packed);
	}

	g_dbg = 0;
	free(vp);
}

int	main(int ac, char **av)
{
	t_data	*data;
	int		fd;

	if (ac < 2)
	{
		fprintf(stderr, "Usage: %s <scene.rt>\n", av[0]);
		return (1);
	}
	init_data(&data);
	validate_args_and_open(data, 2, av, &fd);
	parse(data, fd);
	close(fd);
	init_all(data);

	fprintf(stderr, "Objects: ");
	t_obj *o = data->obj;
	int n = 0;
	while (o) { n++; o = o->next; }
	fprintf(stderr, "%d\n", n);
	fprintf(stderr, "BVH nodes: %d\n", data->bvh_count);

	int test_pixels[][2] = {
		{0, 0}, {1919, 0}, {0, 1079}, {1919, 1079}, {960, 540}
	};
	for (int i = 0; i < 5; i++)
	{
		render_one_pixel(data, test_pixels[i][0], test_pixels[i][1]);
		fprintf(stderr, "\n");
	}
	return (0);
}
