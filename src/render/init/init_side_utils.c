/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_side_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erigonza <erigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 12:39:47 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/18 17:21:58 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_frame	set_frame(t_v3 axis)
{
	t_frame	frame;

	frame.forward = normalize(axis);
	frame.right = normalize(cross(vdefine(1, 0, 0), frame.forward));
	if (vlength(frame.right) < EPSILON)
		frame.right = normalize(cross(vdefine(0, 1, 0), frame.forward));
	frame.up = normalize(cross(frame.forward, frame.right));
	return (frame);
}
