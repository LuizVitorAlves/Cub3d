NAME = cub3d

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc -Ift_printf -g

# Diretório da MLX
MLX_DIR = ./minilibx-linux
MLX = $(MLX_DIR)/libmlx.a

# Diretório do ft_printf
PRINTF_DIR = ./ft_printf
PRINTF = $(PRINTF_DIR)/libftprintf.a

# Flags para linkar a MLX
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -lz

# Lista de arquivos fonte
SRCS = \
	main.c \
	parse_cub.c \
	parse_utils.c \
	gnl_provisoria.c \
	parse_color.c \
	tokenizador.c \
	raycasting.c \
	render_utils.c \
	keys.c \
	door.c \
	sprite.c \
	minimap.c \
	textures.c\
	parser_cub_utils.c\
	parser_cub_utils_suport.c\
	utils_suport_helper.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# -----------------------------

all: $(NAME)

# Compila MLX
$(MLX):
	$(MAKE) -C $(MLX_DIR)

# Compila ft_printf
$(PRINTF):
	$(MAKE) -C $(PRINTF_DIR)

# Executável depende de objetos + MLX + ft_printf
$(NAME): $(OBJS) $(MLX) $(PRINTF)
	$(CC) $(CFLAGS) $(OBJS) $(PRINTF) $(MLX) $(MLX_FLAGS) -o $(NAME)

# Regra para compilar objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(MLX_DIR) clean
	$(MAKE) -C $(PRINTF_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(PRINTF_DIR) fclean

re: fclean all

.PHONY: all clean fclean re