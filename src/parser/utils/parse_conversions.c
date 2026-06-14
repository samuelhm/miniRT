/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_conversions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:47:50 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 12:33:44 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_rgb	colors_parse(t_data *data, char *str, int i)
{
	t_rgb	rgb;
	int		val;

	while (str[i] && ft_isspace(str[i]))
		i++;
	val = ft_atoi_parse(data, str, i, 3);
	rgb.r = (unsigned char)(val > 0 ? val : 1);
	i = skip_color(data, str, i, 0);
	val = ft_atoi_parse(data, str, i, 3);
	rgb.g = (unsigned char)(val > 0 ? val : 1);
	i = skip_color(data, str, i, 0);
	val = ft_atoi_parse(data, str, i, 3);
	rgb.b = (unsigned char)(val > 0 ? val : 1);
	return (rgb);
}

char	*doubles_parse(t_obj *obj, char *str, int i, int flag)
{
	double	x;
	double	y;
	double	z;
	char	*tmp;

	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && (!ft_isdigit(str[i]) && str[i] != '-'))
		exit(er(obj->data, "error: doubles_parse: map parsing:\n", str));
	x = ft_atof(obj->data, str, i);
	i = skip_double(obj->data, str, i, 1);
	y = ft_atof(obj->data, str, i);
	i = random_sum_parse(obj, str, i);
	z = ft_atof(obj->data, str, i);
	i = sum_parse(obj->data, str, i, 0);
	if (flag == 0)
		obj->pos = vdefine(x, y, z);
	else if (flag == 1)
		obj->axis = normalize(vdefine(x, y, z));
	tmp = ft_substr(str, (unsigned int)i, ft_strlen(str));
	return (tmp);
}

int	ft_atoi_parse(t_data *data, char *str, int i, int flag)
{
	int			res;
	int			sign;

	res = 0;
	sign = 1;
	while ((flag >= 1 && flag <= 3) && str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && str[i] == ',' && flag != 2)
		i++;
	else if (str[i] == '-')
		sign = -sign;
	if (flag == 2 && (!ft_isdigit(str[i]) || (str[i + 1] && str[i + 2])))
		exit(er(data, "error: atoi: str invalid\n", NULL));
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		exit(er(data, "error: atoi: not a digit\n", str));
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = (res * 10) + (str[i] - 48);
		if (flag == 3 && (res > 255 || sign == -1))
			exit(er(data, "error: ft_atoi_parse: color not 0-255\n", str));
		i++;
	}
	return (res * sign);
}

double	ft_atof_normi(t_data *data, char *str, int i)
{
	double	fraction;
	double	divisor;

	fraction = 0.0;
	divisor = 10.0;
	while (str[i] >= '0' && str[i] <= '9' && (str[i] != ','
			|| ft_isspace(str[i]) != 1))
	{
		fraction += (str[i] - '0') / divisor;
		divisor *= 10.0;
		i++;
		if (str[i] && !(str[i] == ',' || str[i] == '.' || ft_isspace(str[i])
				|| ft_isdigit(str[i])))
			exit(er(data, "wrong char between nums", str));
	}
	return (fraction);
}

double	ft_atof(t_data *data, char *str, int i)
{
	double		res;
	int			sign;

	res = 0.0;
	sign = 1;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && (str[i] != ','
			|| ft_isspace(str[i]) != 1))
	{
		res = (res * 10.0) + (str[i] - '0');
		i++;
		if (str[i] && !(str[i] == ',' || str[i] == '.' || ft_isspace(str[i])
				|| ft_isdigit(str[i])))
			exit(er(data, "wrong char between nums", str));
	}
	if (str[i] == '.' && (str[i] != ',' || ft_isspace(str[i]) != 1))
		return ((res + ft_atof_normi(data, str, ++i)) * sign);
	return (res * sign);
}
