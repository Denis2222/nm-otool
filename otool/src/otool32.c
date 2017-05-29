/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool32.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:27:26 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 17:24:23 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../otool.h"

void	print_memory_32(t_ofile *ofile, struct section *se)
{
	unsigned char		*str;
	struct mach_header	*mh;
	size_t				c;

	mh = ofile->ptr;
	ft_printf("\nContents of (__TEXT,__text) section\n");
	str = (unsigned char *)((void*)ofile->ptr + toswap32(ofile, se->offset));
	c = 0;
	while (c < toswap32(ofile, se->size))
	{
		ft_printf("%08x", toswap32(ofile, se->addr) + c);
		ft_putchar('	');
		print_line(str, c, toswap32(ofile, se->size), (toswap32(ofile, mh->cputype) == CPU_TYPE_POWERPC));
		c += 16;
	}
}

void	print_section_32(t_ofile *ofile, struct segment_command *sc)
{
	struct section	*se;
	unsigned int	i;

	i = 0;
	se = (void *)sc + sizeof(struct segment_command);
	while (i < toswap32(ofile, sc->nsects))
	{
		if (!ft_strcmp("__text", se->sectname) &&
			!ft_strcmp("__TEXT", se->segname))
			print_memory_32(ofile, se);
		se = (void*)se + sizeof(struct section);
		i++;
	}
}

void	print_segment_32(struct load_command *lc, t_ofile *ofile)
{
	struct segment_command *sc;

	sc = (struct segment_command *)lc;
	print_section_32(ofile, sc);
}

void	handle_32(t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header		*mh;
	struct load_command		*lc;

	mh = (struct mach_header *)ofile->ptr;
	i = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < toswap32(ofile, mh->ncmds))
	{
		if (toswap32(ofile, lc->cmd) == LC_SEGMENT)
			print_segment_32(lc, ofile);
		lc = (void *)lc + toswap32(ofile, lc->cmdsize);
		i++;
	}
}
