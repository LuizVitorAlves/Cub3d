NAME=libftprintf.a
SRC=ft_putnbr.c \
	ft_putnumber_base.c \
	ft_putnumber_base_cap.c\
	ft_strlen.c \
	ft_count_args.c \
	printf.c \
	ft_putchar.c \
	ft_putstr.c \
	ft_putptr.c \
	ft_putnbr_u.c
OBJS=$(SRC:.c=.o)
CC=cc 
CC_FLAGS=  -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $@ $(OBJS)

clean:
	rm -rf $(OBJS) 

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)

.c.o:
	$(CC) $(CC_FLAGS) -c $< -o $@ 
