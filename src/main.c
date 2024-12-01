/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 13:12:46 by erigonza          #+#    #+#             */
/*   Updated: 2024/12/01 13:04:58 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

// make && ./miniRT 0.0 0.0 5.0 1 230 232 255 0.0 -3.0 -1.0 1
int	main(int ac, char **av)
{
	t_data			data;
	mlx_t			*mlx;
    mlx_image_t		*img;
	int				fd;
	t_rgb			**img_rgb;

	// if (ac != 2)
	// 	exit(er("error: 2 args needed", NULL));
	// correct_file(av[1]);
	// fd = open(av[1], O_RDONLY);
	// if (fd < 0)
	// 	exit(er("error: fd filed", NULL));
	//data.obj = parse(&data, data.obj, av, fd);
	mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, "miniRT", true);
	if (!mlx)
		exit (er("Failed to initialize MLX42", NULL));

	img = mlx_new_image(mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!img)
		exit(er("Failed to create image\n", NULL));
	t_data	*prueba;
	prueba = get_test();
	img_rgb = render(prueba, 0, 0);
	for (int i = 0; i < WINDOW_WIDTH; i++)
	{
		for (int j = 0; j < WINDOW_HEIGHT ; j++)
		{
			mlx_put_pixel(img, i, j, get_acolour(0x0, img_rgb[i][j].r, img_rgb[i][j].g, img_rgb[i][j].b));
		}
	}
	// ft_init(&data, av);
	// obj = data.obj;
	// while (obj)
	// {
	// 	// organizator(obj);
	// 	ft_sphere(data.obj, data.sLight, img);
	// 	obj = obj->next;
	// }
    mlx_image_to_window(mlx, img, 0, 0);
	mlx_key_hook(mlx, &my_keyhook, NULL);
    mlx_loop(mlx);
    mlx_delete_image(mlx, img);
    mlx_terminate(mlx);
	exit(0);
}
