# Nome do executável
NAME = cub3d

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

# Diretório da MLX
MLX_DIR = ./minilibx-linux
MLX = $(MLX_DIR)/libmlx.a

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
	render_utils.c

# Arquivos fontes da parte de bônus
SRCS_BONUS = 

# Arquivos objeto correspondentes
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

# -------------------------------------
# Regra principal
all: $(NAME)

# Compila a MLX se ainda não existir
$(MLX):
	$(MAKE) -C $(MLX_DIR)

# Executável depende dos objetos e da MLX
$(NAME): $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(MLX) $(MLX_FLAGS) -o $(NAME)

# Regra para o bônus
bonus: $(OBJS) $(OBJS_BONUS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(OBJS_BONUS) $(MLX) $(MLX_FLAGS) -o $(NAME)_bonus

# Compila os objetos
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa os arquivos .o
clean:
	rm -f $(OBJS) $(OBJS_BONUS)
	$(MAKE) -C $(MLX_DIR) clean

# Limpa tudo, inclusive o executável
fclean: clean
	rm -f $(NAME) $(NAME)_bonus
	$(MAKE) -C $(MLX_DIR) fclean

# Recompila do zero
re: fclean all

.PHONY: all clean fclean re bonus
