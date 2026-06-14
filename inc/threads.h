/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:24:39 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/05 11:09:57 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef THREADS_H
# define THREADS_H

# include <pthread.h>
# include <stdint.h>
# include "rt.h"

constexpr int	NUM_THREADS = 6;

typedef struct s_thread_data
{
	alignas(64) int	thread_id;
	uint32_t		*image;
	t_vp			*vp;
	t_cam			*cam;
	int				mode;
	t_data			*data;
}	t_thread_data;

void	render_with_threads(t_data *data, t_vp *vp, uint32_t *image, int mode);
void	*process_rows(void *arg);

#endif
