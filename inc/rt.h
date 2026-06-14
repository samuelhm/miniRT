/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:55:24 by erigonza          #+#    #+#             */
/*   Updated: 2025/01/19 13:59:56 by shurtado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# define RESET "\033[0m"
# define BOLD "\033[1m"
# define RED_BK "\033[41m" // background
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[1;33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# define IS_SPACE(c) isspace((unsigned char)(c))
# define IS_DIGIT(c) isdigit((unsigned char)(c))
# include <unistd.h>
# include <sys/time.h>
# include "libvct.h"
# include "util.h"
# include "data.h"
# include "render.h"
# include "console.h"
# include "parser.h"
# include "debug.h"
# include "window.h"
# include "bvh.h"

constexpr int W_WH = 1920;
constexpr int W_HG = 1080;

//		Obj
# define SP 0
# define PL 1
# define CY 2
# define CAP 3
# define CO 6
# define CU 7
# define SIDE 8

// Materials
# define MT 0 // Metallic
# define GL 1 // Glass
# define MR 2 // Mirror
# define CB 3 // Checker Board
# define EM 4 // Light Emision
# define BM 5 // Bump Map
# define TX 6 // Textures
# define MAT_DEFAULT -1 // Default diffuse

// Renders
enum render_mode : uint8_t { FAST = 0, ONE, UPDATE };

typedef long long	t_ll;

#define swap(a, b) do { typeof(*(a)) t = *(a); *(a) = *(b); *(b) = t; } while (0)

// Thread-local RNG
void		tls_rng_seed(uint32_t seed);

// Main
void		last_exit(t_data *data);
void		call_render(void *param);
void		mouse_click(mouse_key_t button, action_t action, \
						modifier_key_t mods, void *param);

#endif
