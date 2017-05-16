/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:54:42 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/16 17:24:18 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

typedef struct	s_ofile
{
	void 			*fatptr;
	void			*ptr;
	struct stat		buf;
	char 			*path;
	unsigned int	isfat;
	unsigned int	isfatswap;
	unsigned int	isswap;
	unsigned int 	is32;
	unsigned int	arch;
}					t_ofile;

uint32_t swap32(uint32_t in)
{
	uint32_t out;

	out = ((in & 0xFF) << 24) | ((in & 0xFF00) << 8) | ((in >> 8) & 0xFF00) | ((in >> 24) & 0xFF);
	return (out);
}

uint32_t 	toswap32(t_ofile *ofile, uint32_t var)
{
	if (ofile->isswap)
		return (swap32(var));
	else
		return (var);
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

void	print_line(unsigned char *str, size_t start, size_t max, int isppc)
{
	size_t i;

	i = start;
	while (i < start + 16 && i < max)
	{
		(void)isppc;
		if (isppc)
		{
			ft_puthex(str[i]);
			ft_puthex(str[i+1]);
			ft_puthex(str[i+2]);
			ft_puthex(str[i+3]);
			ft_putchar(' ');
			i = i + 4;
		}
		else
		{
			ft_puthex(str[i++]);
			ft_putchar(' ');
		}
	}
	ft_putchar('\n');
}

void	print_memory_64(const void *addr, size_t size, uint64_t offset)
{
	unsigned char *str;
	//unsigned char *line;
	size_t	c;

	ft_printf(":\nContents of (__TEXT,__text) section\n");
	str = (unsigned char *)addr;
	c = 0;
	while (c < size)
	{
		ft_printf("%016lx", offset + c);
		ft_putchar('	');
		print_line(str, c, size, 0);

		c += 16;
	}
}


void print_section_64(t_ofile *ofile, struct segment_command_64 *sc)
{
	struct section_64	*se;
	unsigned int		i;

	i = 0;
	se = (void *)sc + sizeof(struct segment_command_64);
	while (i < sc->nsects)
	{
		if (!strcmp("__text", se->sectname) && !strcmp("__TEXT", se->segname))
			print_memory_64((void*)ofile->ptr + se->offset, se->size, se->addr);
		se = (void*)se + sizeof(struct section_64);
		i++;
	}
}

void print_segment_64(struct load_command *lc, t_ofile *ofile)
{
	struct segment_command_64 *sc;

	sc = (struct segment_command_64 *)lc;
	print_section_64(ofile, sc);
}

void	handle_64(t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;

	mh = (struct mach_header_64 *) ofile->ptr;
	i = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if(lc->cmd == LC_SEGMENT_64)
			print_segment_64(lc, ofile);
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void	print_memory_32(t_ofile *ofile, struct section *se)
{
	//unsigned char	*line;
	unsigned char	*str;
	size_t			c;
	//size_t			i;

	ft_printf(":\nContents of (__TEXT,__text) section\n");
	str = (unsigned char *)((void*)ofile->ptr + toswap32(ofile, se->offset));
	c = 0;
	//i = 0;
	while (c < toswap32(ofile, se->size))
	{
		ft_printf("%08x", toswap32(ofile, se->addr) + c);
		ft_putchar('	');
/*
		line = ft_memalloc(20+1*sizeof(char));
		memcpy(line, str + c, 20);
		i = 0;
		while (i < 16)
		{
			ft_printf("%02x %02x %02x %02x ", line[i], line[i+1], line[i+2], line[+3]);
			i = i + 4;
		}
		ft_putchar('\n');
		ft_memset(line, 0, 16);*/
		print_line(str, c, toswap32(ofile, se->size), (ofile->arch == CPU_TYPE_POWERPC || ofile->arch == CPU_TYPE_POWERPC64  ));
		c += 16;
	}
}


void print_section_32(t_ofile *ofile, struct segment_command *sc)
{
	struct section	*se;
	unsigned int		i;

	i = 0;
	se = (void *)sc + sizeof(struct segment_command);
	while (i < toswap32(ofile, sc->nsects))
	{
		if (!strcmp("__text", se->sectname) && !strcmp("__TEXT", se->segname))
			print_memory_32(ofile, se);
		se = (void*)se + sizeof(struct section);
		i++;
	}
}

void print_segment_32(struct load_command *lc, t_ofile *ofile)
{
	struct segment_command *sc;

	sc = (struct segment_command *)lc;
	print_section_32(ofile, sc);
}

void	handle_32(t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header	*mh;
	struct load_command		*lc;


	mh = (struct mach_header *) ofile->ptr;
	i = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < toswap32(ofile, mh->ncmds))
	{
		if(toswap32(ofile, lc->cmd) == LC_SEGMENT)
			print_segment_32(lc, ofile);
		lc = (void *) lc + toswap32(ofile, lc->cmdsize);
		i++;
	}
}

void checktype(t_ofile *ofile)
{
	uint32_t        magic;

	magic = *(unsigned int *) ofile->ptr;
	if (magic == MH_MAGIC_64)
	{
		ofile->isswap = 0;
		ofile->is32 = 0;
	}
	else if (magic == MH_CIGAM_64)
	{
		ofile->isswap = 1;
		ofile->is32 = 0;
	}
	else if (magic == MH_MAGIC)
	{
		ofile->isswap = 0;
		ofile->is32 = 1;
	}
	else if (magic == MH_CIGAM)
	{
		ofile->isswap = 1;
		ofile->is32 = 1;
	}
}

void otool(t_ofile *ofile, t_argvise *arg)
{
	struct mach_header *test;

	checktype(ofile);
	test = (void *)ofile->ptr;
	if (ofile->is32 == 0 && ofile->isswap == 0)
		handle_64(ofile);
	else if (ofile->is32 == 1)
		handle_32(ofile);
	(void) arg;
}

void checkfat(t_ofile *ofile)
{
	struct fat_header	*header;

	header = ofile->ptr;
	ofile->fatptr = ofile->ptr;
	ofile->isfat = 0;
	ofile->isfatswap = 0;
	if (header->magic == FAT_CIGAM)
	{
		ofile->isfat = swap32(header->nfat_arch);
		ofile->isfatswap = 1;
	}
	else if (header->magic == FAT_MAGIC)
	{
		ofile->isfat = header->nfat_arch;
		ofile->isfatswap = 0;
	}
}

void show_archtype(cpu_type_t arch)
{
	if (arch == CPU_TYPE_X86 || arch == CPU_TYPE_I386 || arch == CPU_TYPE_X86_64)
		ft_printf("(architecture i386)");
	else if (arch == CPU_TYPE_POWERPC)
		ft_printf("(architecture ppc)");
	else if (arch == CPU_TYPE_POWERPC64)
		ft_printf("(architecture ppc64)");
	else if (arch == CPU_TYPE_ARM)
		ft_printf("(architecture arm)");
	else if (arch == CPU_TYPE_ARM64)
		ft_printf("(architecture arm64)");
	else
		ft_printf("(specific architecture)");
}

void otoolfat(t_ofile *ofile, t_argvise *arg)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	unsigned int		iarch;

	iarch = 0;
	header = ofile->ptr;

	while (iarch < ofile->isfat)
	{
		ft_printf("%s ", ofile->path);
		arch = (ofile->fatptr + sizeof(struct fat_header) + (sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		ofile->arch = swap32(arch->cputype);
		if (ofile->isfatswap)
			show_archtype(swap32(arch->cputype));
		else
			show_archtype(arch->cputype);
		//ft_printf(" %d %p %p", iarch, ofile->ptr, ofile->fatptr);
		otool(ofile, arg);
		iarch++;
	}
}

void ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	(void) i;
	checkfat(ofile);
	// Verifier l'architecture
 	if (ofile->isfat)
		otoolfat(ofile, arg);
	else
	{
		ft_printf("%s", ofile->path);
		otool(ofile, arg);
	}
}

t_ofile *open_ofile(char *path)
{
	int			fd;

	t_ofile		*ofile;
	ofile = (t_ofile*)malloc(sizeof(t_ofile));
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "open fail on %s", path);
		return (NULL);
	}
	ofile->path = path;
	if (fstat(fd, &(ofile->buf)) < 0)
	{
		ft_dprintf(2, "fstat fail on %s", path);
		return (NULL);
	}
	if ((ofile->ptr = mmap(0, ofile->buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "mmap fail on %s", path);
		return (NULL);
	}

	return (ofile);
}

void	close_ofile(t_ofile *ofile)
{
	//ft_printf("Try to close %p      %d", ofile->ptr, ofile->buf.st_size);
	if (munmap(ofile->ptr, ofile->buf.st_size) < 0)
	{
		ft_dprintf(2, "munmap fail on %s", ofile->path);
		return ;
	}
}

int			main(int argc, char **argv)
{
	int			i;
	t_ofile		*ofile;
	t_argvise	*arg;
	arg = new_argvise(argc, argv);

	if (argc <= 1)
	{
		fprintf(stderr, "Please give me argv");
		return (EXIT_FAILURE);
	}

	i = 0;
	while (arg->files[i] != '\0')
	{
		if ((ofile = open_ofile(arg->files[i])))
		{
			//ft_printf("%s ", arg->files[i]);
			ofileheader(ofile, arg, i);
			close_ofile(ofile);
		}
		else
			return (EXIT_FAILURE);
		i++;
	}
	return (0);
}
