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
	gnl_provisoria.c

# Arquivo objeto correspondente
OBJS = $(SRCS:.c=.o)

# Regra principal
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

# Limpa os arquivos .o
clean:
	rm -f $(OBJS)

# Limpa tudo, inclusive o executável
fclean: clean
	rm -f $(NAME)

# Recompila do zero
re: fclean all

.PHONY: all clean fclean re
