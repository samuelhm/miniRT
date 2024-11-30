/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:33:01 by erigonza          #+#    #+#             */
/*   Updated: 2024/11/30 13:35:00 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libvct.h"

t_p3	vdefine(float x, float y, float z)
{
	t_p3	vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return (vector);
}

t_p3	vadd(t_p3 a, t_p3 b)
{
	t_p3	p;

	p.x = a.x + b.x;
	p.y = a.y + b.y;
	p.z = a.z + b.z;
	return (p);
}

t_p3	vsubstract(t_p3 a, t_p3 b)
{
	t_p3	p;

	p.x = a.x - b.x;
	p.y = a.y - b.y;
	p.z = a.z - b.z;
	return (p);
}

float	dot(t_p3 a, t_p3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_p3	cross(t_p3 a, t_p3 b)
{
	t_p3	cp;

	cp.x = a.y * b.z - a.z * b.y;
	cp.y = a.z * b.x - a.x * b.z;
	cp.z = a.x * b.y - a.y * b.x;
	return (cp);
}
