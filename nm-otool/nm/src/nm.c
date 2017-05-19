/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:35 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/19 19:50:08 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"


void	print_output_32(struct symtab_command *sym, t_ofile *ofile)
{
	char			*strtable;
	struct nlist	*symbols;
	unsigned int	i;
	t_symtab		*liste;

	i = 0;
	symbols = (void*)ofile->ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ofile->ptr+sym->stroff; // Location of the string table
	while (i<sym->nsyms)
	{
		if (ft_strlen(strtable + symbols[i].n_un.n_strx) > 0)
			liste = addsymtabsort(&liste, newsymtab(sym, toswap32(ofile, strtable + symbols[i].n_un.n_strx), ofile->ptr, i));
		i++;
	}
	showsymtabs(liste, ofile);
}

void	handle(t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header	*mh;
	struct load_command		*lc;

	mh = (struct mach_header *) ofile->ptr;
	i = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			print_output_32((struct symtab_command *)lc, ofile);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void	print_output(struct symtab_command *sym, t_ofile *ofile)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	t_symtab		*liste;

	i = 0;
	symbols = (void*)ofile->ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ofile->ptr+sym->stroff; // Location of the string table
	while (i<sym->nsyms)
	{
		if (ft_strlen(strtable + symbols[i].n_un.n_strx) > 0)
			liste = addsymtabsort(&liste, newsymtab(sym, strtable + symbols[i].n_un.n_strx, ofile->ptr, i));
		i++;
	}
	showsymtabs(liste, ofile);
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
		if (lc->cmd == LC_SYMTAB)
		{
			print_output((struct symtab_command *)lc, ofile);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(t_ofile *ofile, t_argvise *arg)
{
	struct mach_header *test;

	checktype(ofile);
	test = (void *)ofile->ptr;
	if (ofile->is32 == 0 && ofile->isswap == 0)
		handle_64(ofile);
	else if (ofile->is32 == 1)
		handle(ofile);
	(void) arg;
}

void nmfat(t_ofile *ofile, t_argvise *arg)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	unsigned int		iarch;

	iarch = 0;
	header = ofile->ptr;

	while (iarch < ofile->isfat)
	{
		//ft_printf("%s \n", ofile->path);
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
		nm(ofile, arg);
		iarch++;
	}
}

void ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	(void) i;
	checkfat(ofile);
 	if (ofile->isfat)
		nmfat(ofile, arg);
	else
	{
		//ft_printf("%s", ofile->path);
		nm(ofile, arg);
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
			ofileheader(ofile, arg, i);
			close_ofile(ofile);
		}
		else
			return (EXIT_FAILURE);
		i++;
	}
	return (0);
}
