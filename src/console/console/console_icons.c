/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_icons.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 21:43:07 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 18:28:14 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	put_img_arrows(t_data *data)
{
	mlx_image_t	**icons;
	int			margin;
	int			i;
	int			count;

	margin = BG_WITH - 100;
	icons = data->console->icons;
	i = -1;
	count = count_arrows(data);
	while (++i < count)
	{
		if (i % 4 == 3)
			continue ;
		mlx_image_to_window(data->mlx, icons[2], data->x - \
							margin, 98 + (30 * i));
	}
	i = -1;
	while (++i < count)
	{
		if (i % 4 == 3)
			continue ;
		mlx_image_to_window(data->mlx, icons[3], data->x - \
							icons[1]->width - 25, 98 + (30 * i));
	}
}

void	set_resize_buton_images_normi(t_data *data, mlx_image_t **icons, \
										t_img_btn *img_btn)
{
	int	i;

	i = -1;
	while (++i < 14)
		icons[i] = mlx_texture_to_image(data->mlx, img_btn->iconst[i]);
	mlx_resize_image(icons[0], icons[0]->width / 1.5, icons[0]->height / 1.5);
	mlx_resize_image(icons[1], icons[1]->width / 1.5, icons[1]->height / 1.5);
	mlx_image_to_window(data->mlx, icons[0], data->x - BG_WITH + 30, 0);
	mlx_image_to_window(data->mlx, icons[1], data->x - icons[1]->width - 30, 0);
	mlx_resize_image(icons[2], icons[2]->width / 3, icons[2]->height / 3);
	mlx_resize_image(icons[3], icons[3]->width / 3, icons[3]->height / 3);
	mlx_resize_image(icons[4], icons[4]->width / 3, icons[4]->height / 3);
	mlx_resize_image(icons[5], icons[5]->width / 3, icons[5]->height / 3);
	mlx_resize_image(icons[6], icons[6]->width / 3, icons[6]->height / 3);
	mlx_resize_image(icons[7], icons[7]->width / 3, icons[7]->height / 3);
	mlx_resize_image(icons[8], icons[8]->width / 3, icons[8]->height / 3);
	mlx_resize_image(icons[9], icons[9]->width / 3, icons[9]->height / 3);
	mlx_resize_image(icons[10], icons[10]->width / 3, icons[10]->height / 3);
	mlx_resize_image(icons[11], icons[11]->width / 3, icons[11]->height / 3);
	mlx_resize_image(icons[12], icons[12]->width / 3, icons[12]->height / 3);
	mlx_resize_image(icons[13], icons[13]->width / 3, icons[13]->height / 3);
}

void	set_resize_buton_images(t_data *data, t_img_btn *img_btn)
{
	mlx_image_t	**icons;
	int			i;

	i = -1;
	icons = data->console->icons;
	img_btn->iconst[0] = mlx_load_png("./assets/consol/arrow_b_left.png");
	img_btn->iconst[1] = mlx_load_png("./assets/consol/arrow_b_right.png");
	img_btn->iconst[2] = mlx_load_png("./assets/consol/arrow_s_left_blue.png");
	img_btn->iconst[3] = mlx_load_png("./assets/consol/arrow_s_right_blue.png");
	img_btn->iconst[4] = mlx_load_png("./assets/consol/DF.png");
	img_btn->iconst[5] = mlx_load_png("./assets/consol/MR.png");
	img_btn->iconst[6] = mlx_load_png("./assets/consol/MT.png");
	img_btn->iconst[7] = mlx_load_png("./assets/consol/GL.png");
	img_btn->iconst[8] = mlx_load_png("./assets/consol/EM.png");
	img_btn->iconst[9] = mlx_load_png("./assets/consol/DF_SELECTED.png");
	img_btn->iconst[10] = mlx_load_png("./assets/consol/MR_SELECTED.png");
	img_btn->iconst[11] = mlx_load_png("./assets/consol/MT_SELECTED.png");
	img_btn->iconst[12] = mlx_load_png("./assets/consol/GL_SELECTED.png");
	img_btn->iconst[13] = mlx_load_png("./assets/consol/EM_SELECTED.png");
	while (++i < 14)
		if (!img_btn->iconst[i])
			exit(er(data, "error: set_resize_buton_images: !textura", NULL));
	set_resize_buton_images_normi(data, icons, img_btn);
}

void	fill_image_list(t_data *data, t_img_btn *img_btn)
{
	int	i;
	int	count;

	count = count_labels(data);
	i = -1;
	while (++i < count)
	{
		lst_add_back(&data->console->btn_list, lst_new(img_btn->labels[i]));
		mlx_set_instance_depth(&img_btn->labels[i]->instances[0], 4);
	}
	i = -1;
	while (++i < 4)
	{
		lst_add_back(&data->console->btn_list, \
					lst_new(data->console->icons[i]));
		if (i < 4)
			mlx_set_instance_depth(&data->console->icons[i]->instances[0], 3);
	}
}
