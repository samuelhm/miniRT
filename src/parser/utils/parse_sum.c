/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 13:40:44 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/13 12:14:52 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	random_sum_parse(t_obj *obj, char *str, int i)
{
	int		j;
	int		k;

	j = 0;
	k = 0;
	while (str[i] && str[i] != ',')
	{
		if (IS_DIGIT(str[i]))
			j = 0;
		if (k >= 2 || j >= 2)
			er(obj->data, "error: random_sum_parse", str);
		if (IS_SPACE(str[i]))
			break ;
		if (str[i] == '.')
			j++;
		else if (str[i] == ',')
			k++;
		else if (!IS_DIGIT(str[i]) && str[i] != '-')
			er(obj->data, "error: random_sum_parse", str);
		i++;
	}
	if (str[i] && str[i] == ',')
		i++;
	return (i);
}

// skips 1 double | stops when ' '
int	skip_double(t_data *data, char *str, int i, int flag)
{
	int		j;

	j = 0;
	if (!str[i])
		er(data, "error: skip_double: wrong map: skipdouble", str);
	while (str[i] && IS_SPACE(str[i]))
		i++;
	while (str[i])
	{
		if (flag == 0 && IS_SPACE(str[i]))
			return (i);
		else if (flag == 1 && str[i] == ',')
			return (++i);
		else if (str[i] == '.')
			j++;
		else if (!(IS_DIGIT(str[i]) || str[i] == '-'))
			er(data, "error: skip_double: wrong map: just nums", str);
		if (j > 1)
			er(data, "error: skip_double: wrong map: too many .", str);
		else if (str[i + 1] && str[i] == '.' && !IS_DIGIT(str[i + 1]))
			er(data, "error: skip_double: wrong map: wrong char", str);
		i++;
	}
	return (i);
}

// skips 3 doubles | 1.2,4.1,0.0
// str | i (start) | j . (0) | k , (0)
int	skip_doubles(t_data *data, char *str, int i, int k)
{
	data->j = 0;
	if (!str[i + 1] || !IS_SPACE(str[i + 1]))
		er(data, "error: skip_doubles: wrong map: NULL", NULL);
	while (str[i + 1] && IS_SPACE(str[i + 1]))
		i++;
	while (str[++i])
	{
		if (k == 2 && IS_SPACE(str[i]))
			break ;
		if (str[i] == '.')
			data->j++;
		else if (str[i] == ',')
			k++;
		if ((data->j > 3 || k > 2) || (IS_SPACE(str[i]) && k != 2))
			er(data, "error: skip_doubles: too many , or .", str);
		else if (str[i + 1] && ((str[i] == '.' || str[i] == ',')
				&& (!IS_DIGIT(str[i + 1]) && str[i + 1] != '-')))
			er(data, "error: skip_doubles: after , or .", str);
		else if (str[i + 1] && (str[i] == '-' || str[i] == '+')
			&& !IS_DIGIT(str[i + 1]))
			er(data, "error: skip_doubles: after - or +", str);
	}
	if (!str[i])
		er(data, "error: skip_doubles: wrong map: NULL end", str);
	return (i);
}

// skips 3 colors | 255,23,0
int	skip_color(t_data *data, char *str, int i, int flag)
{
	while (flag == 0 && str[i] && IS_SPACE(str[i]))
		i++;
	if (!str[i] || IS_SPACE(str[i]))
		er(data, "error: skip_color: wrong map", NULL);
	while (str[i])
	{
		if (str[i + 1] && str[i] == ',' && !IS_DIGIT(str[i + 1]))
			er(data, "error: skip_color: wrong map: num after ,", str);
		else if (str[i] == ',')
			return (i + 1);
		else if (!IS_DIGIT(str[i]))
			er(data, "error: skip_color: wrong map: not a num", str);
		i++;
	}
	return (i);
}

// skips 3 doubles | 1.2,4.1,0.0
int	sum_parse(t_data *data, char *str, int i, int j)
{
	while (str[i] && !IS_SPACE(str[i++]))
	{
		if (!IS_DIGIT(str[i - 1]) && str[i - 1] != '.' && str[i - 1] != '-')
			er(data, "error: sum_parse: 2 parsing", str);
		else if (str[i - 1] == '.')
			j++;
	}
	while (str[i] && IS_SPACE(str[i]))
		i++;
	return (i);
}
