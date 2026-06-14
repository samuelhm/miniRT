/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 12:51:59 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/21 21:11:24 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	last_exit(t_data *data)
{
	lst_clear(&data->console->btn_list, del_image);
	lst_clear(&data->console->click_lst, free);
	free(data->m_trace);
	free(data->m_god);
	if (data->img)
		mlx_delete_image(data->mlx, data->img);
	if (data->img_last)
		free_image_all(data, data->img_last);
	mlx_terminate(data->mlx);
	free(data->console);
	free_willy_lst(data);
	exit(0);
}

void	call_render(void *param)
{
	t_data		*data;

	data = (t_data *)param;
	if (data->render_sel)
		data->render_sel(param);
}

void	mouse_click(mouse_key_t button, action_t action, \
						modifier_key_t mods, void *param)
{
	t_data		*data;
	int			x;
	int			y;

	(void)button;
	(void)mods;
	data = param;
	if (action == 1)
	{
		if (data->last_render != FAST)
			data->render_sel = render_fast;
		mlx_get_mouse_pos(data->mlx, &x, &y);
		console_click(data, x, y);
	}
}

int	main(int ac, char **av)
{
	t_data	*data;
	int		fd;

	srand((unsigned int)mlx_get_time());
	init_data(&data);
	validate_args_and_open(data, ac, av, &fd);
	parse(data, fd);
	close(fd);
	init_mlx(data);
	init_all(data);
	data->render_sel = render_fast;
	mlx_resize_hook(data->mlx, &resise_w, data);
	mlx_loop_hook(data->mlx, call_render, data);
	mlx_mouse_hook(data->mlx, mouse_click, data);
	mlx_key_hook(data->mlx, &my_keyhook, data);
	mlx_loop(data->mlx);
}
