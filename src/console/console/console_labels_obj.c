/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console_labels_obj.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 12:32:05 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/21 19:52:33 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	plane_set_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	plane_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Plane", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Axis", mleft + 155, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
	img_btn->labels[7] = put_str(mlx, img_btn->row5, mleft + 35, top + 230);
	img_btn->labels[8] = put_str(mlx, img_btn->row6, mleft + 35, top + 260);
}

void	sphere_set_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	sphere_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Sphere", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Size", mleft + 158, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 25, top + 200);
}

void	cylinder_set_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	cylinder_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Cylinder", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Axis", mleft + 155, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
	img_btn->labels[7] = put_str(mlx, img_btn->row5, mleft + 35, top + 230);
	img_btn->labels[8] = put_str(mlx, img_btn->row6, mleft + 35, top + 260);
	img_btn->labels[9] = put_str(mlx, "Size", mleft + 158, top + 290);
	img_btn->labels[10] = put_str(mlx, img_btn->row7, mleft + 25, top + 320);
	img_btn->labels[11] = put_str(mlx, img_btn->row8, mleft + 25, top + 350);
}

void	cube_set_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	cube_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Cube", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Axis", mleft + 155, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
	img_btn->labels[7] = put_str(mlx, img_btn->row5, mleft + 35, top + 230);
	img_btn->labels[8] = put_str(mlx, img_btn->row6, mleft + 35, top + 260);
	img_btn->labels[9] = put_str(mlx, "Size", mleft + 158, top + 290);
	img_btn->labels[10] = put_str(mlx, img_btn->row7, mleft + 25, top + 320);
	img_btn->labels[11] = put_str(mlx, img_btn->row8, mleft + 25, top + 350);
	img_btn->labels[12] = put_str(mlx, img_btn->row9, mleft + 25, top + 380);
}

void	cone_set_labels(t_data *data, t_img_btn *img_btn, int top)
{
	int			mleft;
	mlx_t		*mlx;

	mlx = data->mlx;
	mleft = data->x - BG_WITH;
	cone_labels(data, img_btn);
	img_btn->labels[0] = put_str(mlx, "Cone", mleft + 120, top - 2);
	img_btn->labels[1] = put_str(mlx, "Pos", mleft + 155, top + 50);
	img_btn->labels[2] = put_str(mlx, img_btn->row1, mleft + 35, top + 80);
	img_btn->labels[3] = put_str(mlx, img_btn->row2, mleft + 35, top + 110);
	img_btn->labels[4] = put_str(mlx, img_btn->row3, mleft + 35, top + 140);
	img_btn->labels[5] = put_str(mlx, "Axis", mleft + 155, top + 170);
	img_btn->labels[6] = put_str(mlx, img_btn->row4, mleft + 35, top + 200);
	img_btn->labels[7] = put_str(mlx, img_btn->row5, mleft + 35, top + 230);
	img_btn->labels[8] = put_str(mlx, img_btn->row6, mleft + 35, top + 260);
	img_btn->labels[9] = put_str(mlx, "Size", mleft + 158, top + 290);
	img_btn->labels[10] = put_str(mlx, img_btn->row7, mleft + 25, top + 320);
	img_btn->labels[11] = put_str(mlx, img_btn->row8, mleft + 25, top + 350);
}
