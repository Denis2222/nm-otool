.PHONY: libft

SRCS = ../src/open.c \
	   ../src/archtype.c \
	   ../src/check.c \
	   ../src/swap.c \
	   ./src/otool.c \
	   ./src/otool32.c \
	   ./src/otool64.c \
	   ./src/print.c

NAME = ft_otool
GCC_FLAG = -Wall -Werror -Wextra
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
