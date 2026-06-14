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

NAME			:= rt

SRC_D			:= ./src/
INC_D			:= ./inc/
OBJ_D			:= $(SRC_D)tmp/
DEBUG_D			:= $(SRC_D)debug_tmp/
# LIBFT removed — using glibc
MLX_D			:= ./lib/MLX42/build/
VCT_D			:= ./lib/libvector/

FILES			:= main.c \
					util/str_sub.c util/str_trim.c util/str_split.c util/str_join.c \
					util/str_cmp.c util/free_strs.c util/gnl.c util/lst.c \
					parser/parser/parser.c \
					parser/obj/parse_obj.c parser/obj/parse_acl.c parser/obj/parse_acl_extra.c \
					parser/utils/parse_sum.c parser/utils/parse_utils.c parser/utils/parse_conversions.c parser/utils/parse_free_willlies.c\
					parser/utils/parse_main_checker.c parser/utils/parse_checker.c \
					parser/extra_funcs/extra_funcs.c parser/extra_funcs/extra_funcs_utils.c \
					render/render/render.c render/render/three_renders.c\
					render/texture/uv_map.c render/texture/uv_obj.c\
					render/bump_map/bm_sphere.c render/bump_map/bm_plane.c \
					render/obj/cylinder.c render/obj/caps.c render/obj/cone.c render/obj/intersections.c render/obj/viewport.c \
					render/illumination/illumination.c \
					render/calcs/calcs.c render/calcs/calcs_utils.c render/calcs/calcs_utils2.c render/calcs/materials.c render/calcs/materials_utils.c render/calcs/quadratic.c \
					render/init/init_image.c render/init/init_general.c \
					render/init/init_obj.c render/init/init_obj_utils.c render/init/init_side.c render/init/init_side_utils.c render/init/init_materials.c \
					render/color/color.c render/color/checker_board.c \
					render/free/free.c \
					render/bvh/bvh_build.c render/bvh/bvh_traverse.c \
					console/console/console_init_extra.c console/console/console_init_pos_axis.c console/console/console_init_general.c console/console/consol_init_rows.c \
					console/console/console_run.c console/console/console_click.c console/console/console_click_obj.c console/console/console_click_obj_utils.c \
					console/console/console_icons.c console/console/console_icon_count.c\
					console/console/console_labels.c console/console/console_labels_obj.c console/console/console_labels_set_obj.c console/console/console_labels_set.c \
					console/console/console_helpers.c console/console/console_helpers2.c \
					console/render/render.c console/console/console_click_utils.c console/console/console_click_cam.c \
					console/console/console_click_alight.c console/console/console_click_slight.c\
					debug/debug_info.c \
					window/mlx.c window/mlx_utils.c

SRCS			:= $(addprefix $(SRC_D), $(FILES))

OBJS			:= $(patsubst $(SRC_D)%.c,$(OBJ_D)%.o,$(SRCS))
DOBJS			:= $(patsubst $(SRC_D)%.c,$(DEBUG_D)%.o,$(SRCS))

CC				:= cc
IFLAGS			:= -I$(INC_D) -I$(VCT_D)

# Base warnings
WFLAGS			:= -Wall -Wextra -Werror

# Extra strict warnings
XFLAGS			:= -Wshadow -Wunused -Wformat=2 -Wundef \
					-Wstrict-prototypes -Wmissing-prototypes \
					-Winit-self -Wimplicit-fallthrough=3 -Wcast-align \
					-Wswitch-default -Wswitch-enum -Wduplicated-branches \
					-Wduplicated-cond -Wlogical-op -Wstack-protector \
					-fstrict-aliasing

# Release flags
CFLAGS			:= -std=gnu23 -O3 -flto -march=native -pthread $(WFLAGS) $(XFLAGS)

# Debug flags
DFLAGS			:= -std=gnu23 -g3 -O0 -pthread $(WFLAGS) $(XFLAGS) \
					-fsanitize=address,undefined,leak,bounds

LDFLAGS			:= -flto -pthread

LIBVCT			:= ./lib/libvector/libvct.a

MLX				:= libmlx42.a
DIR_MLX			:= ./lib/MLX42
MLXFLAGS		:= -ldl -lm -lX11 -lXext
GLFW			:= $(MLX_D)_deps/glfw-build/src/libglfw3.a

RM				:= rm -rf

# ─── Default: release build ───────────────────────────────────────────────────

all:		libmlx libvct $(NAME)

# ─── Debug build ──────────────────────────────────────────────────────────────

debug:			libmlx libvct $(NAME)_debug
	@echo "\033[1;32m[DEBUG BUILD READY]\033[0m"
	@echo "Usage: ./$(NAME)_debug scenes/figuras_pov.rt"

$(NAME)_debug:	$(DOBJS)
	$(CC) $(DFLAGS) $(DOBJS) $(MLX_D)$(MLX) $(GLFW) $(LIBVCT) $(MLXFLAGS) -o $@

$(DEBUG_D)%.o:	$(SRC_D)%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(IFLAGS) $(DFLAGS) -MMD -o $@ -c $<

# ─── Libraries ────────────────────────────────────────────────────────────────

libmlx:
	@cmake ./lib/MLX42 -B $(MLX_D) && make -C $(MLX_D) -j4

libvct:
			@make -s -C ./lib/libvector

# ─── Release objects ──────────────────────────────────────────────────────────

$(OBJ_D)%.o:	$(SRC_D)%.c Makefile
	@printf "\033[0;33m\r🔨 $< ✅ \033[0m"
	@mkdir -p $(dir $@)
	$(CC) $(IFLAGS) $(CFLAGS) -MMD -o $@ -c $<

$(NAME):		$(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_D)$(MLX) $(GLFW) $(LIBVCT) $(MLXFLAGS) -o $(NAME)
	@echo "\n\033[1;32m[✓] $(NAME) built successfully\033[0m"

# ─── Cleaning ─────────────────────────────────────────────────────────────────

c clean:
			@make clean -s -C ./lib/libvector
	@$(RM) $(DIR_MLX)/build
	@$(RM) $(OBJ_D) $(DEBUG_D)

f fclean:		clean
			@make fclean -s -C ./lib/libvector
	@$(RM) $(NAME) $(NAME)_debug

r re:			fclean all

rebug:			fclean debug

# ─── Help ─────────────────────────────────────────────────────────────────────

help:
	@echo "\033[1;36mminiRT Makefile targets:\033[0m"
	@echo "  make          → Release build (-O3 -flto -march=native)"
	@echo "  make debug    → Debug build (-g3 -O0 -fsanitize)"
	@echo "  make clean    → Remove objects"
	@echo "  make fclean   → Remove objects + binary"
	@echo "  make re       → fclean + release"
	@echo "  make rebug    → fclean + debug"
	@echo "  make help     → This message"

# ─── Misc ─────────────────────────────────────────────────────────────────────

-include $(OBJS:.o=.d)
-include $(DOBJS:.o=.d)

.PHONY:		all clean fclean re f c r libmlx libvct debug rebug help

