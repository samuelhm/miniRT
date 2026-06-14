/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_acl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 14:27:07 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 17:23:24 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	create_alight(t_data *data, char *str, int type)
{
	char	*tmp;

	tmp = NULL;
	if (type != 3 || (str[1] && !IS_SPACE(str[1])))
		return ;
	if (data->a_light)
		er(data, "error: create_alight: +	1 ambient light", str);
	data->a_light = calloc(1, sizeof(t_alight));
	data->a_light->exist = 1;
	data->a_light->br = parse_atof(data, str, 1);
	tmp = str_sub(str, (unsigned int)skip_double(data, str, 1, 0), strlen(str));
	data->a_light->rgb = colors_parse(data, tmp, 0);
	check_end(data, tmp, 1);
	free(tmp);
}

void	cam_blur(t_data *data, char *str2, char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i] && IS_SPACE(tmp[i]))
		i++;
	if (tmp[i] && IS_DIGIT(tmp[i]))
	{
		data->cam->focus_dist = parse_atof(data, tmp, 0);
		if (data->cam->focus_dist < 0)
			er(data, "error: cam_blur: focus distance negative", NULL);
		str2 = str_sub(tmp, (unsigned int)skip_double(data, tmp, 0, 0), strlen(tmp));
		data->cam->aperture = parse_atof(data, str2, 0);
		if (str2)
			free(str2);
		if (data->cam->aperture < 0 || data->cam->aperture > 25)
			er(data, "error: cam_blur: aperture >0", tmp);
	}
	else if (tmp[i] && !IS_SPACE(tmp[i]) && tmp[i] != '\n')
		er(data, "error: cam_blur: wrong data after fov", tmp);
	else
	{
		data->cam->focus_dist = -1;
		data->cam->aperture = 0;
	}
}

void	create_cam(t_data *data, char *str, int type)
{
	char	*tmp;
	char	*str2;

	tmp = NULL;
	if (type != 4 || (str[1] && !IS_SPACE(str[1])))
		return ;
	if (data->cam)
		er(data, "error: create_cam: more than 1 camera", NULL);
	data->cam = calloc(1, sizeof(t_cam));
	data->cam->pos = doubles_acl_parse(data, str, 1);
	tmp = str_sub(str, (unsigned int)skip_doubles(data, str, 0, 0), strlen(str));
	data->cam->axis = normalize(doubles_acl_parse(data, tmp, 0));
	str2 = str_sub(tmp, (unsigned int)skip_doubles(data, tmp, -1, 0), strlen(tmp));
	data->cam->fov = parse_atoi(data, str2, 0, 1);
	free(tmp);
	tmp = str_sub(str2, (unsigned int)skip_double(data, str2, 0, 0), strlen(str2));
	cam_blur(data, str2, tmp);
	if (str2)
		free(str2);
	if (tmp)
		free(tmp);
}

static void	create_alight_normi(t_slight **s_light, t_slight *new_light)
{
	t_slight	*current;

	if (*s_light == NULL)
		*s_light = new_light;
	else
	{
		current = *s_light;
		while (current->next != NULL)
			current = current->next;
		current->next = new_light;
	}
}

void	create_slight(t_data *data, t_slight **s_light, char *str, int type)
{
	t_slight	*new_light;
	char		*tmp[2];

	if (type != 5 || (str[1] && !IS_SPACE(str[1])))
		return ;
	new_light = calloc(1, sizeof(t_slight));
	if (!new_light)
		er(data, "error: malloc failed for slight", NULL);
	new_light->pos = doubles_acl_parse(data, str, 1);
	tmp[0] = str_sub(str, (unsigned int)skip_doubles(data, str, 0, 0), strlen(str));
	new_light->br = parse_atof(data, tmp[0], 0);
	tmp[1] = str_sub(tmp[0], (unsigned int)skip_double(data, tmp[0], 0, 0), \
											strlen(tmp[0]));
	new_light->rgb = colors_parse(data, tmp[1], 0);
	check_end(data, tmp[1], 0);
	new_light->next = NULL;
	create_alight_normi(s_light, new_light);
	free(tmp[0]);
	free(tmp[1]);
}
