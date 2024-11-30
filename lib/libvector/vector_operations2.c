/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:32:52 by erigonza          #+#    #+#             */
/*   Updated: 2024/11/30 13:35:05 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libvct.h"

float		mod(t_p3 v)
{
	return (sqrt(dot(v, v)));
}

t_p3		normalize(t_p3 p)
{
	t_p3	nv;
	float	mod;

	mod = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
	nv.x = p.x / mod;
	nv.y = p.y / mod;
	nv.z = p.z / mod;
	return (nv);
}

float		vsin(t_p3 a, t_p3 b)
{
	return (sqrt(1 - pow(vcos(a, b), 2)));
}

float		vcos(t_p3 a, t_p3 b)
{
	return (dot(a, b) / (mod(a) * mod(b)));
}

t_p3		scal_x_vec(float n, t_p3 p)
{
	t_p3	v;

	v.x = n * p.x;
	v.y = n * p.y;
	v.z = n * p.z;
	return (v);
}
