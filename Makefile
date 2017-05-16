.PHONY: libft

SRCS = ft_otool.c

NAME = ft_otool
GCC_FLAG = -Wall -Werror -Wextra
CC = gcc $(GCC_FLAG)

OBJS = $(SRCS:.c=.o)

all: libft $(NAME)

libft:
	make -C ./libft/

$(%.o): $(%.c)
	$(CC) -o $@ -c $^

$(NAME): $(OBJS)
	$(CC) -o $@ $^ -Llibft -lft

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make -C libft/ fclean

nm: libft
	gcc -I ./libft/ -L libft -lft nm.c -o ft_nm

re: fclean all
