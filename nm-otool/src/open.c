/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 13:49:12 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 08:59:14 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm-otool.h"

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

uint64_t swap64( uint64_t val )
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL ) | ((val >> 8) & 0x00FF00FF00FF00FFULL );
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL ) | ((val >> 16) & 0x0000FFFF0000FFFFULL );
    return (val << 32) | (val >> 32);
}

uint64_t 	toswap64(t_ofile *ofile, uint64_t var)
{
	if (ofile->isswap)
		return (swap64(var));
	else
		return (var);
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

int checktype(t_ofile *ofile)
{
	uint32_t        magic;

	magic = *(unsigned int *) ofile->ptr;
	if (magic == MH_MAGIC_64)
	{
		ofile->isswap = 0;
		ofile->is32 = 0;
		return (1);
	}
	else if (magic == MH_CIGAM_64)
	{
		ofile->isswap = 1;
		ofile->is32 = 0;
		return (1);
	}
	else if (magic == MH_MAGIC)
	{
		ofile->isswap = 0;
		ofile->is32 = 1;
		return (1);
	}
	else if (magic == MH_CIGAM)
	{
		ofile->isswap = 1;
		ofile->is32 = 1;
		return (1);
	}
	return (0);
}

void show_archtype(cpu_type_t arch)
{
	if (arch == CPU_TYPE_X86_64)
		ft_printf("(architecture x86_64)");
	else if (arch == CPU_TYPE_X86 || arch == CPU_TYPE_I386)
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


void show_archtypefor(cpu_type_t arch)
{
	if (arch == CPU_TYPE_X86_64)
		ft_printf("(for architecture x86_64)");
	else if (arch == CPU_TYPE_X86 || arch == CPU_TYPE_I386)
		ft_printf("(for architecture i386)");
	else if (arch == CPU_TYPE_POWERPC)
		ft_printf("(for architecture ppc)");
	else if (arch == CPU_TYPE_POWERPC64)
		ft_printf("(for architecture ppc64)");
	else if (arch == CPU_TYPE_ARM)
		ft_printf("(for architecture arm)");
	else if (arch == CPU_TYPE_ARM64)
		ft_printf("(for architecture arm64)");
	else
		ft_printf("(for specific architecture)");
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
	if (ofile->buf.st_size < (long long)sizeof(struct mach_header))
	{
		ft_dprintf(2, "Invalid file format %s\n", path);
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
	if (munmap(ofile->ptr, ofile->buf.st_size) < 0)
	{
		ft_dprintf(2, "munmap fail on %s", ofile->path);
		return ;
	}
}
