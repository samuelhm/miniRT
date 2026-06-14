#ifndef BVH_H
# define BVH_H

# include "rt.h"

# define BVH_LEAF_SIZE 4

typedef struct s_aabb
{
	t_v3	min;
	t_v3	max;
}	t_aabb;

typedef struct s_bvh_node
{
	t_aabb	box;
	int		left;
	int		right;
	int		obj_start;
	int		obj_count;
}	t_bvh_node;

void	build_bvh(t_data *data);
t_obj	*bvh_traverse(t_data *restrict data, t_ray *restrict ray,
			double *restrict t_min);
void	free_bvh(t_data *data);

#endif
