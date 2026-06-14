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

#include "miniRT.h"

void	create_alight(t_data *data, char *str, int type)
{
	char	*tmp;

	tmp = NULL;
	if (type != 3 || (str[1] && !ft_isspace(str[1])))
		return ;
	if (data->a_light)
		exit(er(data, "error: create_alight: +	1 ambient light", str));
	data->a_light = calloc(1, sizeof(t_alight));
	data->a_light->exist = 1;
	data->a_light->br = ft_atof(data, str, 1);
	tmp = ft_substr(str, skip_double(data, str, 1, 0), ft_strlen(str));
	data->a_light->rgb = colors_parse(data, tmp, 0);
	check_end(data, tmp, 1);
	free(tmp);
}

void	cam_blur(t_data *data, char *str2, char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i] && ft_isspace(tmp[i]))
		i++;
	if (tmp[i] && ft_isdigit(tmp[i]))
	{
		data->cam->focus_dist = ft_atof(data, tmp, 0);
		if (data->cam->focus_dist < 0)
			exit(er(data, "error: cam_blur: focus distance negative", NULL));
		str2 = ft_substr(tmp, skip_double(data, tmp, 0, 0), ft_strlen(tmp));
		data->cam->aperture = ft_atof(data, str2, 0);
		if (str2)
			free(str2);
		if (data->cam->aperture < 0 || data->cam->aperture > 25)
			exit(er(data, "error: cam_blur: aperture >0", tmp));
	}
	else if (tmp[i] && !ft_isspace(tmp[i]) && tmp[i] != '\n')
		exit(er(data, "error: cam_blur: wrong data after fov", tmp));
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
	if (type != 4 || (str[1] && !ft_isspace(str[1])))
		return ;
	if (data->cam)
		exit(er(data, "error: create_cam: more than 1 camera", NULL));
	data->cam = calloc(1, sizeof(t_cam));
	data->cam->pos = doubles_acl_parse(data, str, 1);
	data->cam->pos.z += 200;
	tmp = ft_substr(str, skip_doubles(data, str, 0, 0), ft_strlen(str));
	data->cam->axis = normalize(doubles_acl_parse(data, tmp, 0));
	str2 = ft_substr(tmp, skip_doubles(data, tmp, -1, 0), ft_strlen(tmp));
	data->cam->fov = ft_atoi_parse(data, str2, 0, 1);
	free(tmp);
	tmp = ft_substr(str2, skip_double(data, str2, 0, 0), ft_strlen(str2));
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

	if (type != 5 || (str[1] && !ft_isspace(str[1])))
		return ;
	new_light = calloc(1, sizeof(t_slight));
	if (!new_light)
		exit(er(data, "error: malloc failed for slight", NULL));
	new_light->pos = doubles_acl_parse(data, str, 1);
	tmp[0] = ft_substr(str, skip_doubles(data, str, 0, 0), ft_strlen(str));
	new_light->br = ft_atof(data, tmp[0], 0);
	tmp[1] = ft_substr(tmp[0], skip_double(data, tmp[0], 0, 0), \
											ft_strlen(tmp[0]));
	new_light->rgb = colors_parse(data, tmp[1], 0);
	check_end(data, tmp[1], 0);
	new_light->next = NULL;
	create_alight_normi(s_light, new_light);
	return (free(tmp[0]), free(tmp[1]));
}
