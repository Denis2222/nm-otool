.PHONY: libft

SRCS = ../src/open.c \
	   ./src/t_symtab.c \
	   ./src/nm.c

NAME = ft_nm
GCC_FLAG = -Wall -Werror -Wextra -g
CC = gcc $(GCC_FLAG)

OBJS = $(SRCS:.c=.o)

all: libft $(NAME)

libft:
	make -C ../libft/

$(%.o): $(%.c)
	$(CC) -I. -o $@ -c $^

$(NAME): $(OBJS)
	$(CC) -o $@ $^ -L../libft -lft -I ./

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	make -C ../libft/ fclean

re: fclean all
