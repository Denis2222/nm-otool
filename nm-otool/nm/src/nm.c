/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:35 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 15:39:31 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

void	print_output(struct symtab_command *sym, char *ptr)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	t_symtab		*liste;

	i = 0;
	symbols = (void*)ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ptr+sym->stroff; // Location of the string table
	while (i<sym->nsyms)
	{
		liste = addsymtabsort(&liste, newsymtab(sym, strtable + symbols[i].n_un.n_strx, ptr, i));
		i++;
	}
	showsymtabs(liste);
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
