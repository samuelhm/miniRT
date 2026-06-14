#include "rt.h"
#include <stdio.h>

int	main(int ac, char **av)
{
	t_data	*data;
	int		fd;
	t_vp	*vp;
	t_cam	*cam;

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

	cam = data->cam;
	data->x = 1920;
	data->y = 1080;
	vp = init_viewport(cam, data->x, data->y);
	if (!vp) { fprintf(stderr, "viewport failed\n"); return (1); }

	int misses = 0;
	int blacks = 0;
	int strip_h = data->y / 6;

	for (int y = 0; y < data->y; y += 10)
	{
		for (int x = 0; x < data->x; x += 10)
		{
			tls_rng_seed((uint32_t)(x * 2654435761U + y));
			t_ray	ray = generate_ray(data, vp, cam, x, y, 1);
			double	t_min = INFINITY;
			t_obj	*hit = find_closest(data, &ray, &t_min);
			if (!hit)
			{
				misses++;
				if (misses <= 5)
					fprintf(stderr, "MISS px=(%d,%d) d=(%.6f,%.6f,%.6f)\n",
						x, y, ray.direction.x, ray.direction.y,
						ray.direction.z);
			}
			else
			{
				uint32_t c = trace_ray(ray, data);
				if (c == BLACK)
				{
					blacks++;
					if (blacks <= 5)
						fprintf(stderr, "BLACK px=(%d,%d) hit=%d t=%.2f\n",
							x, y, hit->type, t_min);
				}
			}
		}
	}

	fprintf(stderr, "Scan: %d misses, %d blacks out of %d pixels\n",
		misses, blacks,
		(data->x / 10) * (data->y / 10));
	free(vp);
	return (0);
}
