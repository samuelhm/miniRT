/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_funcs_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:02:22 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 16:57:03 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_materials(t_obj *obj)
{
	obj->material.m_type = -1;
	obj->material.reflectivity = 0;
	obj->material.transmittance = 0;
	obj->material.roughness = 1;
	obj->material.absorption = 1;
	obj->material.specularity = 0.1;
	obj->material.board_scale = -1;
}

void	skip_colors(t_data *data, char *str, char **res)
{
	int		i;

	i = 0;
	while (str[i] && isspace(str[i]))
		i++;
	i = skip_color(data, str, i, 0);
	i = skip_color(data, str, i, 0);
	while (str[i] && isdigit(str[i]))
		i++;
	while (str[i] && isspace(str[i]))
		i++;
	*res = str_sub(str, (unsigned int)i, strlen(str));
}

int	type_extra_func(char *str)
{
	static const char	*bts[] = {"mt", "gl", "mr", "cb", "em", "bm", "tx", \
																NULL};
	int				i;

	i = 0;
	while (bts[i] && !strcmp(bts[i], str))
		i++;
	if (bts[i])
		return (i);
	return (-1);
}

void	parse_tx_and_bm(t_obj *obj, char **args, char *tmp, int i)
{
	obj->material.bm_size = (unsigned int)parse_atoi(obj->data, args[i + 3], 0, 0);
	tmp = str_trim(args[i + 4], " \n\t");
	obj->material.bm_texture = mlx_load_png(tmp);
	free(tmp);
	if (!obj->material.bm_texture)
		exit(er(obj->data, "error: parse_tx: texture not valid", NULL));
	else if (args[i + 5])
		exit(er(obj->data, "error: parse_tx: wrong arg", NULL));
}

void	parse_bm_and_tx(t_obj *obj, char **args, char *tmp, int i)
{
	obj->material.tx_size = (unsigned int)parse_atoi(obj->data, args[i + 3], 0, 0);
	tmp = str_trim(args[i + 4], " \n\t");
	obj->material.texture = mlx_load_png(tmp);
	free(tmp);
	if (!obj->material.texture)
		exit(er(obj->data, "error: parse_bm: texture not valid", NULL));
	else if (args[i + 5])
		exit(er(obj->data, "error: parse_bm: wrong arg", NULL));
}
