/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_labels.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 20:02:25 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/21 19:51:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	set_cam_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	write_cam_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Camera", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Axis", mleft + 155, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
	img_btn->labels[7] = put_str(mlx, img_btn->row5, mleft + 35, top + 230);
	img_btn->labels[8] = put_str(mlx, img_btn->row6, mleft + 35, top + 260);
	img_btn->labels[9] = put_str(mlx, "Others", mleft + 145, top + 290);
	img_btn->labels[10] = put_str(mlx, img_btn->row7, mleft + 25, top + 320);
	img_btn->labels[11] = put_str(mlx, img_btn->row8, mleft + 25, top + 350);
	img_btn->labels[12] = put_str(mlx, img_btn->row9, mleft + 25, top + 380);
}

void	set_alight_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	write_alight_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Ambient Light", mleft + 85, top - 2);
	img_btn->labels[1] = put_str(mlx, "BRIGHTNESS", mleft + 125, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
}

void	set_slight_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	write_slight_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Spot Lights", mleft + 92, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "BRIGHTNESS", mleft + 125, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
}
