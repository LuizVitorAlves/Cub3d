NAME = cub3d

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc -Ift_printf

# Diretório da MLX
MLX_DIR = ./minilibx-linux
MLX = $(MLX_DIR)/libmlx.a

# Diretório do printf
PRINTF_DIR = ./printf
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
	textures.c

# Arquivos objeto
OBJS = $(SRCS:.c=.o)

# -----------------------------

all: $(NAME)

# Compila MLX
$(MLX):
	$(MAKE) -C $(MLX_DIR)

# Compila printf
$(PRINTF):
	$(MAKE) -C $(PRINTF_DIR)

# Executável depende de objetos + MLX + printf
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