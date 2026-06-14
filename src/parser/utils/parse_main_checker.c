/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 17:18:36 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/18 18:06:48 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	check_params(t_data *data)
{
	t_obj	*obj;

	obj = data->obj;
	if (obj)
		check_obj(obj);
	else
		er(data, "error: check_params: obj not exist", NULL);
	check_params_acl(data, data->cam);
}
