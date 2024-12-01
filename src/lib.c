/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 12:38:00 by erigonza          #+#    #+#             */
/*   Updated: 2024/12/01 12:02:23 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

t_v3	vDefine(float x, float y, float z)
{
	t_v3	vector;

	vector.x = x;
	vector.y = y;
	vector.z = z;
	return (vector);
}

// // Function to calculate the dot product of two vectors
// // This helps us determine angles between directions, which we'll need for shading
// float	dot(t_v3 a, t_v3 b)
// {
//     return (a.x * b.x + a.y * b.y + a.z * b.z);
// }

// // Function to subtract two vectors (essentially finding the difference between two points)
// // We'll use this to find distances between points, like from the camera to the sphere
t_v3	subtract(t_v3 a, t_v3 b)
{
    t_v3	result;

    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return (result);
}
