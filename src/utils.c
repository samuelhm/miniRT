/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:47:50 by erigonza          #+#    #+#             */
/*   Updatej: 2024/11/23 15:01:11 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

t_obj	*newObj(t_obj *obj)
{
	t_obj	*tmp;

	tmp = malloc(sizeof(*tmp));
	if (!tmp)
		return (NULL);
	tmp->next = NULL;
	return (tmp);
}

int	er(char *s, char *argv)
{
	ft_printf(2, "%s", RED);
	if (s)
		ft_printf(2, "%s", s);
	ft_printf(2, "%s", BOLD);
	if (argv)
		ft_printf(2, " %s", argv);
	ft_printf(2, "%s", RESET);
	return (1);
}

float	ft_atof_normi(char *str, int i)
{
	float	fraction;
	float	divisor;

	fraction = 0.0;
	divisor = 10.0;
	while (str[i] >= '0' && str[i] <= '9' && (str[i] != ','
			|| ft_isspace(str[i]) != 1))
	{
		fraction += (str[i] - '0') / divisor;
		divisor *= 10.0f;
		i++;
	}
	return (fraction);
}

float	ft_atof(char *str, int i) // i = start
{
	float res;
	int sign;

	res = 0.0;
	sign = 1;
	while (str[i] && ft_isspace(str[2]) == 1)
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && (str[i] != ','
			|| ft_isspace(str[i]) != 1))
	{
		res = (res * 10.0f) + (str[i] - '0');
		i++;
	}
	// printf("i -> %d\n", i);
	if (str[i] == '.' && (str[i] != ',' || ft_isspace(str[i]) != 1))
		return ((res + ft_atof_normi(str, ++i)) * sign);
	return (res * sign);
}

// void	ft_init(t_data *data, char **av)
// {
// 	data->obj->ray_start = vDefine(0.0, 0.0, 0.0);			// Camera position (where our rays start from)
//  	data->obj->sphere_radius = 1.4;						// Radius (size) of the sphere
//     data->obj->sphere_center = vDefine(
//         atof(av[1]),
//         atof(av[2]),
//         atof(av[3]));

//     data->obj->sphere_radius = atof(av[4]);
//     data->obj->color = ((atoi(av[5]) << 16) | (atoi(av[6]) << 8) | atoi(av[7]));

//     // Initialize light source
//     data->sLight->pos = vDefine(
//         atof(av[8]),  // X
//         atof(av[9]),  // Y
//         atof(av[10])  // Z
//     );
//     data->sLight->br = atof(av[11]);  // Light brightness ratio
//     if (data->sLight->br < 0.0f || data->sLight->br > 1.0f)
//         er("error: %s: brightness must be in range [0.0, 1.0]", av[11]);
// }
