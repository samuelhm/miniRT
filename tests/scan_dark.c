#include "rt.h"
#include <stdio.h>

int	main(int ac, char **av)
{
	t_data	*data;
	int		fd;
	t_vp	*vp;
	t_cam	*cam;

	if (ac < 2) { fprintf(stderr, "Usage: %s <scene.rt>\n", av[0]); return (1); }
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

	int misses = 0, blacks = 0;
	int dark_r = 255, dark_g = 255, dark_b = 255;
	int dark_x = 0, dark_y = 0;

	for (int y = 0; y < data->y; y++)
	{
		for (int x = 0; x < data->x; x++)
		{
			tls_rng_seed((uint32_t)(x * 2654435761U + y));
			t_ray	ray = generate_ray(data, vp, cam, x, y, 1);
			double	t_min = INFINITY;
			t_obj	*hit = find_closest(data, &ray, &t_min);
			if (!hit) { misses++; continue; }
			uint32_t c = trace_ray(ray, data);
			if (c == BLACK) { blacks++; continue; }
			t_rgb rgb;
			unpack(c, &rgb);
			int sum = (int)rgb.r + (int)rgb.g + (int)rgb.b;
			if (sum < dark_r + dark_g + dark_b)
			{
				dark_r = rgb.r; dark_g = rgb.g; dark_b = rgb.b;
				dark_x = x; dark_y = y;
			}
		}
	}

	int total = data->x * data->y;
	fprintf(stderr, "Total: %d pixels, misses: %d, blacks: %d\n", total, misses, blacks);
	fprintf(stderr, "Darkest pixel: (%d,%d) rgb(%d,%d,%d) sum=%d\n",
		dark_x, dark_y, dark_r, dark_g, dark_b, dark_r+dark_g+dark_b);
	free(vp);
	return (0);
}
