# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: shurtado <shurtado@student.42barcelona.fr> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/19 12:51:14 by shurtado          #+#    #+#              #
#    Updated: 2025/01/24 11:08:33 by shurtado         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= miniRT

SRC_D			:= ./src/
INC_D			:= ./inc/
OBJ_D			:= $(SRC_D)tmp/
LIBFT_D			:= ./lib/libft/
MLX_D			:= ./lib/MLX42/build/
VCT_D			:= ./lib/libvector/

FILES			:= main.c \
					parser/parser/parser.c \
					parser/obj/parse_obj.c parser/obj/parse_acl.c parser/obj/parse_acl_extra.c \
					parser/utils/parse_sum.c parser/utils/parse_utils.c parser/utils/parse_conversions.c parser/utils/parse_free_willlies.c\
					parser/utils/parse_main_checker.c parser/utils/parse_checker.c \
					parser/extra_funcs/extra_funcs.c parser/extra_funcs/extra_funcs_utils.c \
					render/render/render.c render/render/three_renders.c\
					render/texture/uv_map.c render/texture/uv_obj.c\
					render/bump_map/bm_sphere.c render/bump_map/bm_plane.c \
					render/obj/cylinder.c render/obj/caps.c render/obj/cone.c render/obj/intersections.c render/obj/viewport.c \
					render/illumination/illumination.c render/illumination/specular.c \
					render/calcs/calcs.c render/calcs/calcs_utils.c render/calcs/calcs_utils2.c render/calcs/materials.c render/calcs/materials_utils.c render/calcs/quadratic.c \
					render/init/init_image.c render/init/init_rays.c render/init/init_rays_utils.c render/init/init_general.c \
					render/init/init_obj.c render/init/init_obj_utils.c render/init/init_side.c render/init/init_side_utils.c render/init/init_materials.c \
					render/color/color.c render/color/checker_board.c \
					render/free/free.c \
					console/console/console_init_extra.c console/console/console_init_pos_axis.c console/console/console_init_general.c console/console/consol_init_rows.c \
					console/console/console_run.c console/console/console_click.c console/console/console_click_obj.c console/console/console_click_obj_utils.c \
					console/console/console_icons.c console/console/console_icon_count.c\
					console/console/console_labels.c console/console/console_labels_obj.c console/console/console_labels_set_obj.c console/console/console_labels_set.c \
					console/console/console_helpers.c console/console/console_helpers2.c \
					console/render/render.c console/render/init_rays.c console/console/console_click_utils.c console/console/console_click_cam.c \
					console/console/console_click_alight.c console/console/console_click_slight.c\
					debug/debug_info.c \
					window/mlx.c window/mlx_utils.c

SRCS			:= $(addprefix $(SRC_D), $(FILES))

OBJS			:= $(patsubst $(SRC_D)%.c,$(OBJ_D)%.o,$(SRCS))

CC				:= cc
IFLAGS			:= -I$(INC_D) -I$(VCT_D) -I$(LIBFT_D)inc
CFLAGS			:= -g -Ofast -Wall -Wextra -Werror -pthread #-fsanitize=address

LIB				:= lib/

LIBFT			:= libft.a
LIBVCT          := ./lib/libvector/libvct.a

MLX				:= libmlx42.a
DIR_MLX			:= ./lib/MLX42
MLXFLAGS		:= -ldl -lm -lX11 -lXext
GLFW			:= $(MLX_D)_deps/glfw-build/src/libglfw3.a

RM				:= rm -rf

all:		libmlx libs $(NAME)

libmlx:
			@cmake ./lib/MLX42 -B $(MLX_D) && make -C $(MLX_D) -j4

libs:
			@make -s -C $(LIB)libft
			@make -s -C $(LIB)libvector

$(OBJ_D)%.o:	$(SRC_D)%.c Makefile
			printf "\033[0;33m\r🔨 $< ✅ \033[0m"
			@mkdir -p $(dir $@)
			$(CC) $(IFLAGS) $(CFLAGS) -MMD -o $@ -c $<

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) $(OBJS) $(LIBFT_D)$(LIBFT) $(MLX_D)$(MLX) $(GLFW) $(LIBVCT) $(MLXFLAGS) -o $(NAME)
			# @clear

c clean:
			@make clean -s -C $(LIB)libft
			@make clean -s -C $(LIB)libvector
			@$(RM) $(DIR_MLX)/build
			${RM} ./src/tmp
			# @clear

f fclean:		clean
			@make fclean -s -C $(LIB)libft
			@make fclean -s -C $(LIB)libvector
			@${RM} ${NAME}

r re:			fclean all

-include $(OBJS:.o=.d)

.PHONY:		all clean fclean re f c r libs libmlx
