#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>

void	print_output(struct symtab_command *sym, char *ptr)
{
	char			*strtable;
	struct nlist_64	*array;
	unsigned int	i;

	i = 0;
	array = (void*)ptr+sym->symoff;
	strtable = (void*)ptr+sym->stroff;
	printf("\n============%#x=================\nOutput symtab_command:\n", array[i].n_type);
	while (i<sym->nsyms)
	{
		//printf("name:%s sym->cmd:%d  n_type:%#x n_value:%0.16llx \n", strtable + array[i].n_un.n_strx, sym->cmd, (array[i].n_type & N_TYPE), array[i].n_value);
		if ((array[i].n_type & N_TYPE) == N_UNDF)
			printf("                 %s  \n", strtable + array[i].n_un.n_strx);
		else if ((array[i].n_type & N_TYPE) == N_SECT)
			printf("%0.16llx %s\n", array[i].n_value, strtable + array[i].n_un.n_strx);
		i++;
	}
}

void	handle_64(char *ptr)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;
	struct symtab_command	*sym;

	mh = (struct mach_header_64 *) ptr;
	printf("==================\ninfo mach_header:\ncputype:%d\nfiletype:%d\n",mh->cputype, mh->filetype);
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
		else
		{
			printf("]]]]]]]]]]]]]]]load_command->cmd:%#x\n", lc->cmd);
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(char *ptr)
{
	unsigned int	magic_number;

	magic_number = *(unsigned int *) ptr;
	printf("MAGIC_NUMBER:%#x \n", magic_number);
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
		puts("Je suis un binaire 64 bits");
	} else if (magic_number == FAT_MAGIC_64)
	{
		
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
