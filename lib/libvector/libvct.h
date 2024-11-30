/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libvct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:33:09 by erigonza          #+#    #+#             */
/*   Updated: 2024/11/30 13:57:57 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBVCT_H
# define LIBVCT_H

# include <math.h>

typedef struct s_v3
{
	float			x;
	float			y;
	float			z;
}					t_v3;

typedef t_v3	t_p3;

t_p3			vdefine(float x, float y, float z);

t_p3			vadd(t_p3 a, t_p3 b);

t_p3			vsubstract(t_p3 a, t_p3 b);

t_p3			vproduct(t_p3 a, t_p3 b);

float			dot(t_p3 a, t_p3 b);

t_p3			cross(t_p3 a, t_p3 b);

float			mod(t_p3 v);

t_p3			normalize(t_p3 p);

float			vsin(t_p3 a, t_p3 b);

float			vcos(t_p3 a, t_p3 b);

t_p3			scal_x_vec(float n, t_p3 p);

float			distance(t_p3 p1, t_p3 p2);

t_p3			x_axis_rotation(t_p3 vec, float angle);

t_p3			y_axis_rotation(t_p3 vec, float angle);

t_p3			z_axis_rotation(t_p3 vec, float angle);

#endif
