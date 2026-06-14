/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   console.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 18:12:24 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/19 19:25:30 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONSOLE_H
# define CONSOLE_H

# include "rt.h"

# define CAMPLUS 0.5
# define CAMAXISP 0.05f
# define CAMAXISMIN -1.00f
# define CAMAXISMAX 1.00f
# define CAMAPERTURE 0.5f

# define LIGHTPLUS 0.02f
# define LIGHTMIN 0.0f
# define LIGHTMAX 1.0f

# define SPOTPLUS 0.5

# define OBJPLUS 0.5
# define OBJAXISP 0.1f
# define OBJAXISMIN -1.00f
# define OBJAXISMAX 1.00f

# define ALIGHT_M 0
# define CAM_M 1
# define OBJ_M 2
# define SPOT_M 3

# define BG_WITH 300

typedef enum btn_name
{
	none = 0,
	left,
	right,
	row1l,
	row1r,
	row2l,
	row2r,
	row3l = 8,
	row3r,
	row4l,
	row4r,
	row5l,
	row5r,
	row6l,
	row6r,
	row7l,
	row7r,
	row8l,
	row8r,
	row9l,
	row9r,
	pos_xmin = 3,
	pos_xmax = 4,
	pos_ymin = 5,
	pos_ymax = 6,
	pos_zmin = 8,
	pos_zmax = 9,
	axis_xmin,
	axis_xmax,
	axis_ymin,
	axis_ymax,
	axis_zmin,
	axis_zmax,
	fov_min = row7l,
	fov_max = row7r,
	size_max = 0,
	size_min = 0,
	height_max = 0,
	height_min = 0,
	br_min = 3,
	br_max,
	btn_df = 22,
	btn_mr,
	btn_mt,
	btn_gl,
	btn_em
}					t_btn_name;

typedef struct s_btn
{
	t_btn_name		name;
	int				x;
	int				y;
	int				w;
	int				h;
}					t_btn;

typedef struct s_img_btn
{
	char			row1[200];
	char			row2[200];
	char			row3[200];
	char			row4[200];
	char			row5[200];
	char			row6[200];
	char			row7[200];
	char			row8[200];
	char			row9[200];
	mlx_image_t		*labels[50];
	mlx_texture_t	*iconst[14];
}					t_img_btn;

//		console_run
void				run_console(t_data *data);
void				set_all_buttons(t_data *data);
void				set_labels(t_data *data, t_img_btn *img_btn, int top);
void				set_obj_labels(t_data *data, t_img_btn *img_btn, int top);
void				del_image(void *content);

//		init_general
void				click_obj_console(t_data *data, t_btn_name clicked);
void				init_console_normi(t_data *data);
void				init_console(t_data *data);

//		init_pos_axis
void				make_arrows(t_data *data);
void				rows_1(t_data *data);
void				rows_2(t_data *data);
void				rows_3(t_data *data);
void				rows_4(t_data *data);

//		consol_init_rows
void				rows_5(t_data *data);
void				rows_6(t_data *data);

//		init_extra
void				rows_7(t_data *data);
void				rows_8(t_data *data);
void				rows_9(t_data *data);
void				row_material(t_data *data, int click);

//		helpers
mlx_image_t			*create_button(mlx_t *mlx, const char *label, int x, int y);
mlx_image_t			*create_menu_background(t_data *data);
void				set_background(t_data *data);
mlx_image_t			*put_str(mlx_t *mlx, const char *str, int x, int y);

//		labels
void				set_cam_labels(t_data *data, t_img_btn *img_btn, int top);
void				set_alight_labels(t_data *data, t_img_btn *img_btn,
						int top);
void				set_slight_labels(t_data *data, t_img_btn *img_btn,
						int top);

//		console_labels_object
void				plane_labels(t_data *data, t_img_btn *img_btn);
void				sphere_labels(t_data *data, t_img_btn *img_btn);
void				cylinder_labels(t_data *data, t_img_btn *img_btn);
void				cube_labels(t_data *data, t_img_btn *img_btn);
void				cone_labels(t_data *data, t_img_btn *img_btn);

//		console_labels_obj
void				plane_set_labels(t_data *data, t_img_btn *img_btn, int top);
void				sphere_set_labels(t_data *data, t_img_btn *img_btn,
						int top);
void				cylinder_set_labels(t_data *data, t_img_btn *img_btn,
						int top);
void				cube_set_labels(t_data *data, t_img_btn *img_btn, int top);
void				cone_set_labels(t_data *data, t_img_btn *img_btn, int top);

//		write_labels_set
void				write_cam_labels(t_data *data, t_img_btn *img_btn);
void				write_alight_labels(t_data *data, t_img_btn *img_btn);
void				write_slight_labels(t_data *data, t_img_btn *img_btn);
void				write_obj_labels(t_data *data, t_img_btn *img_btn);

//		icons
void				put_img_arrows(t_data *data);
void				set_resize_buton_images_normi(t_data *data, \
						mlx_image_t **icons, t_img_btn *img_btn);
void				set_resize_buton_images(t_data *data, t_img_btn *img_btn);
void				fill_image_list(t_data *data, t_img_btn *img_btn);

//		icons_count
int					count_labels(t_data *data);
int					count_arrows(t_data *data);

//		click
void				console_click(t_data *data, int x, int y);

//		click_obj
void				click_type(t_data *data, t_obj *obj, t_btn_name clicked,
						int type);
void				click_cube(t_data *data, t_obj *obj, t_btn_name clicked);
void				click_cy_co(t_data *data, t_obj *obj, t_btn_name clicked);
void				click_sphere(t_data *data, t_obj *obj, t_btn_name clicked);

//		click_obj_utils
bool				obj_click_move(t_data *data, t_obj *obj,
						t_btn_name clicked);
bool				obj_click_move_normi(t_data *data, t_obj *obj,
						t_slight *slight);

//		click_cam
void				console_click_axis(t_data *data, t_btn_name clicked);
void				console_click_pos(t_data *data, t_btn_name clicked);
void				click_cam_console(t_data *data, t_btn_name clicked);

//		click_alight
bool				alight_click_move(t_data *data, t_btn_name clicked);
void				click_alight_console(t_data *data, t_btn_name clicked);

//		click_slight
bool				slight_click_move(t_data *data, t_slight *slight,
						t_btn_name clicked);
void				click_slight_console(t_data *data, t_slight *slight,
						t_btn_name clicked);

//		click_utils
int					obj_remove(t_obj **lst, t_obj *node);
void				free_after_click(t_data *data);
t_btn_name			get_clicked_btn(t_data *data, int x, int y);
t_slight			*get_prev(t_data *data, t_slight *slight);

//		helpers2
int					console_get_k(t_obj *obj, int i);
void				put_img_buttons(t_data *data);

#endif
