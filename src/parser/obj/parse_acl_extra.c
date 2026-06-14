/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_acl_extra.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:02:22 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/13 10:58:15 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_v3	doubles_acl_parse(t_data *data, char *str, int i)
{
	double	x;
	double	y;
	double	z;

	while (str[i] && IS_SPACE(str[i]))
		i++;
	if (str[i] && !IS_DIGIT(str[i]) && str[i] != '-')
		exit(er(data, "error: doubles_acl_parse: map parsing:\n", str));
	x = parse_atof(data, str, i);
	i = skip_double(data, str, i, 1);
	y = parse_atof(data, str, i);
	i = random_sum_parse(data->obj, str, i);
	z = parse_atof(data, str, i);
	return (vdefine(x, y, z));
}
