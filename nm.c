#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <xlocale.h>
#include "libft.h"

typedef struct				s_loadcom
{
	struct symtab_command	*sym;
	char					*name;
	char					*ptr;
	unsigned int			i;
	struct s_loadcom 		*next;
}							t_loadcom;

t_loadcom *newloadcom(struct symtab_command *sym, char *name, char *ptr, unsigned int i)
{
	t_loadcom *new;

	new = (t_loadcom*)malloc(sizeof(t_loadcom)+1);
	new->sym = sym;
	new->name = strdup(name);
	new->ptr = ptr;
	new->i = i;
	new->next = NULL;
	return (new);
}

t_loadcom *addloadcomsort(t_loadcom **liste, t_loadcom *new)
{
	t_loadcom *begin;
	t_loadcom *current;
	t_loadcom *prev;
	t_loadcom *tmp;

	begin = *liste;
	current = *liste;
	if (current)
	{
		if (ft_strcmp(current->name, new->name) > 0) //New < 1st
		{
			begin  = new;
			begin->next = current;
			return (begin);
		}
		while (current->next)
		{
			if (ft_strcmp(current->next->name, new->name) > 0) // New < next
			{
				tmp = current->next;
				current->next = new;
				new->next = tmp;
				return (begin);
			}
			prev = current;
			current = current->next;
		}
		current->next = new;
	} else
	{
		begin = new;
	}
	//printf("addloadcom ");
	return (begin);
}

void showsym(t_loadcom *s)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int 	i;
	char			c;
	i = s->i;
	symbols = (void*)s->ptr+s->sym->symoff; // Symbol table start location
	strtable = (void*)s->ptr+s->sym->stroff; // Location of the string table
	//printf("\n============%#x=================\nOutput symtab_command:\n", array[i].n_type);

		c = symbols[i].n_type;

		switch(c & N_TYPE)
		{
			case N_UNDF:
				c = 'u';
				if(symbols[i].n_value != 0)
					c = 'c';
			break;
			case N_PBUD:
				c = 'u';
			break;
			case N_ABS:
				c = 'a';
			break;
			case N_SECT:
				//findtypeofsection(symbols[i].n_sect, ptr);
				if(symbols[i].n_sect == 0)
					c = 't';
				else if(symbols[i].n_sect == 1)
					c = 'd';
				else if(symbols[i].n_sect == 2)
					c = 'b';
				else
					c = 's';
			break;
			case N_INDR:
				c = 'i';
			break;
			default:
				c = '?';
			break;
		}
		if((symbols[i].n_type & N_EXT) && c != '?')
				c = toupper(c);
		//printf(" %c ", c);

		//printf("name:%s sym->cmd:%d  n_type:%#x n_value:%0.16llx \n", strtable + array[i].n_un.n_strx, sym->cmd, (array[i].n_type & N_TYPE), array[i].n_value);
		if ((symbols[i].n_type & N_TYPE) == N_UNDF)
			printf("                 %c %s  \n", c, strtable + symbols[i].n_un.n_strx);
		else if ((symbols[i].n_type & N_TYPE) == N_SECT)
			printf("%0.16llx %c %s\n", symbols[i].n_value, c, strtable + symbols[i].n_un.n_strx);

}

void showloadcoms(t_loadcom *liste)
{
	t_loadcom *begin;
	t_loadcom *current;

	begin = liste;
	current = liste;

		while (current)
		{
			showsym(current);
			current = current->next;
		}
}

void	print_output(struct symtab_command *sym, char *ptr)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	t_loadcom		*liste;

	i = 0;
	symbols = (void*)ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ptr+sym->stroff; // Location of the string table
	while (i<sym->nsyms)
	{
		liste = addloadcomsort(&liste, newloadcom(sym, strtable + symbols[i].n_un.n_strx, ptr, i));
		i++;
	}
	showloadcoms(liste);
}

void	handle_64(char *ptr)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;
	struct symtab_command	*sym;



	mh = (struct mach_header_64 *) ptr;
	i = 0;
	lc = (void*)(ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;

			print_output(sym, ptr);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(unsigned int *) ptr;
	//printf("MAGIC_NUMBER get:%#x            REAL MH_MAGIC_64:%#x  FAT_MAGIC:%#x\n", magic_number, MH_MAGIC_64, FAT_MAGIC);
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
		puts("Je suis un binaire 64 bits");
	}
	else if (magic_number == FAT_MAGIC)
	{
		printf("FAT_MAGIC");
	}
	else
	{
		printf("%#x", magic_number);
	}
}

int main(int ac, char **av)
{
	int fd;
	char *ptr;
	struct stat buf;

	if (ac != 2)
	{
		fprintf(stderr, "Please give me argv");
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
