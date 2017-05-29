/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool64.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:26:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 13:50:29 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../otool.h"

void	print_memory_64(const void *addr, size_t size, uint64_t offset)
{
	unsigned char	*str;
	size_t			c;

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

void	print_section_64(t_ofile *ofile, struct segment_command_64 *sc)
{
	struct section_64	*se;
	unsigned int		i;

	i = 0;
	se = (void *)sc + sizeof(struct segment_command_64);
	while (i < sc->nsects)
	{
		if (!ft_strcmp("__text", se->sectname) &&
			!ft_strcmp("__TEXT", se->segname))
			print_memory_64((void*)ofile->ptr + se->offset, se->size, se->addr);
		se = (void*)se + sizeof(struct section_64);
		i++;
	}
}

void	print_segment_64(struct load_command *lc, t_ofile *ofile)
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

	mh = (struct mach_header_64 *)ofile->ptr;
	i = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			print_segment_64(lc, ofile);
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}
