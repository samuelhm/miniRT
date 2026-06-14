#include "bvh.h"

static double	hit_aabb(t_ray *restrict ray, t_aabb box)
{
	double	tmin;
	double	tmax;
	double	t1;
	double	t2;
	double	inv_d;

	inv_d = 1.0 / ray->direction.x;
	t1 = (box.min.x - ray->origin.x) * inv_d;
	t2 = (box.max.x - ray->origin.x) * inv_d;
	tmin = fmin(t1, t2);
	tmax = fmax(t1, t2);
	inv_d = 1.0 / ray->direction.y;
	t1 = (box.min.y - ray->origin.y) * inv_d;
	t2 = (box.max.y - ray->origin.y) * inv_d;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	inv_d = 1.0 / ray->direction.z;
	t1 = (box.min.z - ray->origin.z) * inv_d;
	t2 = (box.max.z - ray->origin.z) * inv_d;
	tmin = fmax(tmin, fmin(t1, t2));
	tmax = fmin(tmax, fmax(t1, t2));
	if (tmax >= fmax(tmin, 0.0))
		return (tmin);
	return (-1.0);
}

t_obj	*bvh_traverse(t_data *restrict data, t_ray *restrict ray,
		double *restrict t_min)
{
	t_bvh_node	*nodes;
	t_obj		**objs;
	int			stack[64];
	int			sp;
	int			node_i;
	int			i;
	t_obj		*closest;

	nodes = data->bvh_nodes;
	objs = data->bvh_objs;
	closest = NULL;
	sp = 0;
	stack[sp++] = 0;
	while (sp > 0)
	{
		node_i = stack[--sp];
		if (hit_aabb(ray, nodes[node_i].box) < 0)
			continue ;
		if (nodes[node_i].left == -1)
		{
			i = nodes[node_i].obj_start - 1;
			while (++i < nodes[node_i].obj_start
				+ nodes[node_i].obj_count)
			{
				double	t;
				t_obj	*obj;

				obj = objs[i];
				t = *t_min;
				if ((obj->type == SP && hit_sp(ray, obj, &t))
					|| ((obj->type == PL || obj->type == SIDE)
						&& hit_pl(data, ray, obj, &t))
					|| (obj->type == CY && hit_cy(ray, obj, &t))
					|| (obj->type == CAP
						&& hit_cap(data, ray, obj, &t))
					|| (obj->type == CO && hit_cone(ray, obj, &t)))
				{
					if (t > 0 && t < *t_min)
					{
						*t_min = t;
						closest = obj;
					}
				}
			}
		}
		else
		{
			stack[sp++] = nodes[node_i].left;
			stack[sp++] = nodes[node_i].right;
		}
	}
	return (closest);
}
