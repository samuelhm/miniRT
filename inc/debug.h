/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 13:40:17 by shurtado          #+#    #+#             */
/*   Updated: 2025/01/18 18:18:03 by erigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEBUG_H
# define DEBUG_H

# include "rt.h"

// debug_info
void		print_t_rgb(const char *label, t_rgb rgb);
void		print_t_v3(const char *label, t_v3 vec);
long long	current_timestamp(void);
void		print_objects(t_obj *obj);

#endif
