# Nome do executável
NAME = cub3d

# Compilador e flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinc

# Lista de arquivos fonte
SRCS = \
	main.c \
	parse_cub.c \
	parse_utils.c \
	gnl_provisoria.c \
	parse_color.c \
	tokenizador.c \
	raycasting.c

# Arquivos fontes da parte de bônus
SRCS_BONUS = 

# Arquivo objeto correspondente
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

# Regra principal
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Regra para o bônus
bonus: $(OBJS) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS) $(OBJS_BONUS) -o $(NAME)_bonus

# Limpa os arquivos .o
clean:
	rm -f $(OBJS) $(OBJS_BONUS)

# Limpa tudo, inclusive o executável
fclean: clean
	rm -f $(NAME) $(NAME)_bonus

# Recompila do zero
re: fclean all

.PHONY: all clean fclean re bonus