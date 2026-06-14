/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 15:35:03 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/23 11:12:10 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_H
# define WINDOW_H

# include "rt.h"

//		mlx
bool	press_keyhook_normi(t_data *data, keys_t key, bool mode);
void	press_keyhook(t_data *data, mlx_key_data_t keydata);
void	my_keyhook(mlx_key_data_t keydata, void *param);
void	resise_w(int32_t width, int32_t height, void *param);
void	key_cam_move(t_data *data, mlx_key_data_t keydata);

//		mlx_utils
void	fill_image(t_data *data, uint32_t *pixels, uint32_t **img_rgb);
void	swap_mgod(t_data *data);
void	set_last(t_data *data);
void	swap_flag_mlx(t_data *data);

#endif
