#include "bvh.h"
#include <stdlib.h>
#include <math.h>

static t_v3	ortho_u(t_v3 a)
{
	t_v3	u;

	if (fabs(a.x) < 0.9)
	{
		u.x = a.y;
		u.y = -a.x;
		u.z = 0;
	}
	else
	{
		u.x = 0;
		u.y = a.z;
		u.z = -a.y;
	}
	return (normalize(u));
}

static void	aabb_grow(t_aabb *box, t_v3 p)
{
	if (p.x < box->min.x)
		box->min.x = p.x;
	if (p.y < box->min.y)
		box->min.y = p.y;
	if (p.z < box->min.z)
		box->min.z = p.z;
	if (p.x > box->max.x)
		box->max.x = p.x;
	if (p.y > box->max.y)
		box->max.y = p.y;
	if (p.z > box->max.z)
		box->max.z = p.z;
}

static t_aabb	aabb_empty(void)
{
	t_aabb	box;

	box.min.x = INFINITY;
	box.min.y = INFINITY;
	box.min.z = INFINITY;
	box.max.x = -INFINITY;
	box.max.y = -INFINITY;
	box.max.z = -INFINITY;
	return (box);
}

static void	aabb_add_disk(t_aabb *box, t_v3 center, t_v3 u, t_v3 v,
		double r)
{
	aabb_grow(box, vadd(center, vmul(r, u)));
	aabb_grow(box, vadd(center, vmul(-r, u)));
	aabb_grow(box, vadd(center, vmul(r, v)));
	aabb_grow(box, vadd(center, vmul(-r, v)));
}

static t_aabb	obj_aabb(t_obj *obj)
{
	t_aabb	box;
	t_v3	u;
	t_v3	v;
	double	r;
	double	h;

	box = aabb_empty();
	if (obj->type == SP)
	{
		r = obj->calcs.radius;
		aabb_grow(&box, vsub(obj->pos, (t_v3){r, r, r}));
		aabb_grow(&box, vadd(obj->pos, (t_v3){r, r, r}));
	}
	else if (obj->type == PL || obj->type == SIDE)
	{
		aabb_grow(&box, (t_v3){-1e4, -1e4, -1e4});
		aabb_grow(&box, (t_v3){1e4, 1e4, 1e4});
	}
	else if (obj->type == CY)
	{
		r = obj->calcs.radius;
		h = obj->calcs.half_height;
		u = ortho_u(obj->axis);
		v = normalize(cross(obj->axis, u));
		aabb_add_disk(&box, vsub(obj->pos, vmul(h, obj->axis)), u, v, r);
		aabb_add_disk(&box, vadd(obj->pos, vmul(h, obj->axis)), u, v, r);
	}
	else if (obj->type == CAP)
	{
		r = obj->calcs.radius;
		u = ortho_u(obj->axis);
		v = normalize(cross(obj->axis, u));
		aabb_add_disk(&box, obj->pos, u, v, r);
	}
	else if (obj->type == CO)
	{
		r = obj->calcs.radius;
		h = obj->height;
		u = ortho_u(obj->axis);
		v = normalize(cross(obj->axis, u));
		aabb_add_disk(&box, obj->pos, u, v, r);
		aabb_grow(&box, vadd(obj->pos, vmul(h, obj->axis)));
	}
	else if (obj->type == CU)
	{
		t_v3	c = obj->pos;
		t_v3	hs = obj->calcs.half_size;

		aabb_grow(&box, vadd(c, hs));
		aabb_grow(&box, vsub(c, hs));
	}
	return (box);
}

static t_v3	aabb_centroid(t_aabb box)
{
	return (vmul(0.5, vadd(box.min, box.max)));
}

static int	partition(t_obj **objs, t_v3 *cents, int lo, int hi, int axis,
		double pivot)
{
	int	i;
	int	j;

	i = lo;
	j = lo;
	while (j <= hi)
	{
		if (((double *)&cents[j])[axis] < pivot)
		{
			swap(&objs[i], &objs[j]);
			swap(&cents[i], &cents[j]);
			i++;
		}
		j++;
	}
	return (i);
}

static void	build_recursive(t_bvh_node *nodes, int *nc, t_obj **objs,
		t_v3 *cents, int lo, int hi)
{
	int		node_i;
	int		count;
	int		axis;
	int		mid;
	double	pivot;
	t_aabb	box;
	int		i;

	node_i = (*nc)++;
	nodes[node_i].left = -1;
	nodes[node_i].right = -1;
	count = hi - lo + 1;
	box = aabb_empty();
	i = lo - 1;
	while (++i <= hi)
	{
		t_aabb	obox = obj_aabb(objs[i]);

		aabb_grow(&box, obox.min);
		aabb_grow(&box, obox.max);
	}
	nodes[node_i].box = box;
	if (count <= BVH_LEAF_SIZE)
	{
		nodes[node_i].obj_start = lo;
		nodes[node_i].obj_count = count;
		return ;
	}
	axis = 0;
	if (box.max.y - box.min.y > box.max.x - box.min.x)
		axis = 1;
	if (box.max.z - box.min.z > (axis ? box.max.y - box.min.y
			: box.max.x - box.min.x))
		axis = 2;
	mid = lo + count / 2;
	pivot = ((double *)&cents[mid])[axis];
	mid = partition(objs, cents, lo, hi, axis, pivot);
	if (mid == lo || mid > hi)
	{
		nodes[node_i].obj_start = lo;
		nodes[node_i].obj_count = count;
		return ;
	}
	nodes[node_i].left = *nc;
	build_recursive(nodes, nc, objs, cents, lo, mid - 1);
	nodes[node_i].right = *nc;
	build_recursive(nodes, nc, objs, cents, mid, hi);
}

static t_obj	**obj_array(t_data *data, int *count)
{
	t_obj	**arr;
	t_obj	*obj;
	int		i;

	*count = 0;
	obj = data->obj;
	while (obj)
	{
		(*count)++;
		obj = obj->next;
	}
	arr = calloc(*count, sizeof(t_obj *));
	if (!arr)
		return (NULL);
	obj = data->obj;
	i = 0;
	while (obj)
	{
		arr[i++] = obj;
		obj = obj->next;
	}
	return (arr);
}

void	build_bvh(t_data *data)
{
	t_obj	**objs;
	t_v3	*cents;
	int		count;
	int		nc;

	if (!data->obj)
		return ;
	objs = obj_array(data, &count);
	if (!objs)
		return ;
	cents = calloc(count, sizeof(t_v3));
	if (!cents)
	{
		free(objs);
		return ;
	}
	data->bvh_nodes = calloc(count * 2, sizeof(t_bvh_node));
	if (!data->bvh_nodes)
	{
		free(objs);
		free(cents);
		return ;
	}
	for (int i = 0; i < count; i++)
		cents[i] = aabb_centroid(obj_aabb(objs[i]));
	nc = 0;
	build_recursive(data->bvh_nodes, &nc, objs, cents, 0, count - 1);
	data->bvh_count = nc;
	data->bvh_objs = objs;
	free(cents);
}

void	free_bvh(t_data *data)
{
	free(data->bvh_nodes);
	free(data->bvh_objs);
	data->bvh_nodes = NULL;
	data->bvh_objs = NULL;
	data->bvh_count = 0;
}
