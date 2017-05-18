#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>


void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	ft_putascii(unsigned char c)
{
	if (c > 31 && c < 127)
		ft_putchar(c);
	else
		ft_putchar('.');
}

void	ft_puthex(unsigned char c)
{
	char tab[16] = "0123456789abcdef";

	ft_putchar(tab[c / 16]);
	ft_putchar(tab[c % 16]);
}

void	print_line(unsigned char *str, size_t start, size_t max)
{
	size_t i;
	char *line;


	asprintf(&line,"%.16zx	", start);
	write(1, line, strlen(line));
	free(line);
	i = start;
	while (i < start + 16 && i < max)
	{
		ft_puthex(str[i]);
		//if (i % 1)
			ft_putchar(' ');
		i++;
	}
	while ( i < start + 16)
	{
		ft_putchar(' ');
		ft_putchar(' ');
		//if (i % 1)
			ft_putchar(' ');
		i++;
	}

	i = start;
	while(i < start + 16 && i < max)
	{
		//ft_putascii(str[i]);
		i++;
	}
	ft_putchar(' ');
	ft_putchar('\n');
}

void	print_memory(const void *addr, size_t size)
{
	unsigned char *str;
	size_t	c;

	str = (unsigned char *)addr;
	c = 0;

	while (c < size)
	{
		print_line(str, c, size);
		c += 16;
	}
}





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
			printf("                 U %s  \n", strtable + array[i].n_un.n_strx);
		else if ((array[i].n_type & N_TYPE) == N_SECT)
		{
			printf("%0.16llx T %s   %p \n", array[i].n_value, strtable + array[i].n_un.n_strx, ptr+array[i].n_value);
		}
		i++;
	}
	//print_memory(ptr, 2048);
}

void print_section(char *ptr, struct segment_command_64 *sc)
{
	struct section_64 *se;
	int		i;

	i = 0;
	se = (void *)sc + sizeof(struct segment_command_64);
	while (i < sc->nsects)
	{
		printf("Section\n");
		printf("%9s %s\n", "sectname", se->sectname);
		printf("%9s %s\n", "segname", se->segname);
		printf("%9s 0x%0.16llx\n", "addr", se->addr);
		printf("%9s 0x%0.16llx\n", "size", se->size);
		printf("%9s %d\n", "offset", se->offset);
		/*if (!strcmp("__text", se->sectname) && !strcmp("__TEXT", se->segname))
			print_memory((void*)ptr + se->offset, se->size);*/
		printf("%9s 2^%d (%.0f)\n", "align", se->align, pow(2,se->align));
		printf("%9s %d\n", "reloff", se->reloff);
		printf("%9s %d\n", "nreloc", se->nreloc);
		printf("%9s 0x%x\n", "flags", se->flags);
		printf("%9s %d\n", "reserved1", se->reserved1);
		printf("%9s %d\n", "reserved2", se->reserved2);
		se = (void*)se + sizeof(struct section_64);
		i++;
	}
}

void print_segment_64(struct load_command *lc, char *ptr)
{
	struct segment_command_64 *sc;

	sc = (struct segment_command_64 *)lc;
	printf("%9s %u\n", "cmd", sc->cmd);
	printf("%9s %d\n", "cmdsize", sc->cmdsize);
	printf("%9s %s\n", "segname", sc->segname);
	printf("%9s 0x%0.16llx\n", "vmaddr", sc->vmaddr);
	printf("%9s 0x%0.16llx\n", "vmsize", sc->vmsize);
	printf("%9s %llu\n", "fileoff", sc->fileoff);
	printf("%9s %llu\n", "filesize", sc->filesize);
	printf("%9s 0x%0.8x\n", "maxprot", sc->maxprot);
	printf("%9s 0x%0.8x\n", "initprot", sc->initprot);
	printf("%9s %d\n", "nsects", sc->nsects);
	printf("%9s 0x%x\n", "flags", sc->flags);

	print_section(ptr, sc);
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
			printf("\n===LC_SYMTAB [%d]\n\n", i);
			sym = (struct symtab_command *)lc;
			print_output(sym, ptr);
			//print_segment_64(lc, ptr);
		}
		else if(lc->cmd == LC_SEGMENT_64)
		{
			printf("\n===LC_SEGMENT_64 [%d]\n\n", i);
			print_segment_64(lc, ptr);
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(unsigned int *) ptr;
	printf("MAGIC_NUMBER get:%#x            REAL MH_MAGIC_64:%#x  FAT_MAGIC:%#x\n", magic_number, MH_MAGIC_64, FAT_MAGIC);
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
		puts("Je suis un binaire 64 bits");
	}
	else if (magic_number == FAT_CIGAM)
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

	if (ac <= 2)
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
