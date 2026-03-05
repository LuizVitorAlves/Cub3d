NAME = cub3d

# ==============================
# COMPILER
# ==============================

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc -Ift_printf -g

# ==============================
# MLX
# ==============================

MLX_DIR = ./minilibx-linux
MLX = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

# ==============================
# FT_PRINTF
# ==============================

PRINTF_DIR = ./ft_printf
PRINTF = $(PRINTF_DIR)/libftprintf.a

# ==============================
# BASE FILES (sempre compilam)
# ==============================

BASE_SRCS = \
	main.c \
	parse_cub.c \
	parse_utils.c \
	gnl/get_next_line.c \
	gnl/get_next_line_utils.c \
	parse_color.c \
	tokenizador.c \
	raycasting.c \
	render_utils.c \
	keys.c \
	textures.c \
	parser_cub_utils.c \
	parser_cub_utils_suport.c \
	utils_suport_helper.c \
	validate_map.c \
	keys_utils.c \
	parser_utils2.c \
	raycasting_utils.c \
	dda.c

# ==============================
# STUB (mandatory)
# ==============================

STUB_SRCS = \
	cub_stub.c \
	cub_stub_utils.c

# ==============================
# BONUS FILES
# ==============================

BONUS_SRCS = \
	bonus/door_bonus.c \
	bonus/gun_draw_bonus.c \
	bonus/minimap_bonus.c \
	bonus/minimap_utils_bonus.c \
	bonus/sprite_bonus.c

# ==============================
# OBJECTS
# ==============================

BASE_OBJS = $(BASE_SRCS:.c=.o)
STUB_OBJS = $(STUB_SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

MANDATORY_OBJS = $(BASE_OBJS) $(STUB_OBJS)
BONUS_BUILD_OBJS = $(BASE_OBJS) $(BONUS_OBJS)

# ==============================
# RULES
# ==============================

all: $(NAME)

# -------- MANDATORY --------

$(NAME): $(MANDATORY_OBJS) $(MLX) $(PRINTF)
	$(CC) $(CFLAGS) $(MANDATORY_OBJS) $(PRINTF) $(MLX) $(MLX_FLAGS) -o $(NAME)

# -------- BONUS --------

bonus: fclean $(MLX) $(PRINTF) $(BONUS_BUILD_OBJS)
	$(CC) $(CFLAGS) $(BONUS_BUILD_OBJS) $(PRINTF) $(MLX) $(MLX_FLAGS) -o $(NAME)

# -------- COMPILE RULE --------

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# -------- LIBS --------

$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(PRINTF):
	$(MAKE) -C $(PRINTF_DIR)

# ==============================
# CLEAN
# ==============================

clean:
	rm -f $(BASE_OBJS) $(STUB_OBJS) $(BONUS_OBJS)
	$(MAKE) -C $(MLX_DIR) clean
	$(MAKE) -C $(PRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re bonus