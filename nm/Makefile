.PHONY: libft

SRCS = ../src/open.c \
	   ../src/archtype.c \
	   ../src/check.c \
	   ../src/swap.c \
	   ./src/t_symtab.c \
	   ./src/t_symtab_64.c \
	   ./src/t_symtab_32.c \
	   ./src/nm.c \
	   ./src/handle.c

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
