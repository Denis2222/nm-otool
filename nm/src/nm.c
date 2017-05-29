/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:35 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 09:16:55 by dmoureu-         ###   ########.fr       */
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
	symbols = (void*)ofile->ptr+toswap32(ofile,sym->symoff); // Symbol table start location
	strtable = (void*)ofile->ptr+toswap32(ofile,sym->stroff); // Location of the string table

	//return ;
	liste = NULL;
	while (i < toswap32(ofile,sym->nsyms))
	{
		if (ft_strlen(strtable + toswap32(ofile,symbols[i].n_un.n_strx)) > 0)
		{
			//ft_printf("[%d]", i);
			liste = addsymtabsort(&liste, newsymtab(sym, strtable + toswap32(ofile,symbols[i].n_un.n_strx), ofile->ptr, i));
			//ft_printf("special : %s\n", strtable + toswap32(ofile,symbols[i].n_un.n_strx));
		}
		i++;
	}
	showsymtabs(liste, ofile);
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
		if (toswap32(ofile,lc->cmd) == LC_SYMTAB)
		{
			print_output_32((struct symtab_command *)lc, ofile);
			break;
		}
		lc = (void *) lc + toswap32(ofile,lc->cmdsize);
		i++;
	}
}

void	print_output_64(struct symtab_command *sym, t_ofile *ofile)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	t_symtab		*liste;

	i = 0;
	symbols = (void*)ofile->ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ofile->ptr+sym->stroff; // Location of the string table
	liste = NULL;
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
			print_output_64((struct symtab_command *)lc, ofile);
			break;
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(t_ofile *ofile, t_argvise *arg, int i, int show)
{
	struct mach_header *test;

	(void)i;
	if (!checktype(ofile))
	{
		ft_dprintf(2, "Not mach-o header can't explore");
		return ;
	}
	test = (void *)ofile->ptr;
	(void)show;
	if (arg->nfiles > 1 && show)
		ft_printf("\n%s:\n", arg->files[i]);
	if (ofile->is32 == 0 && ofile->isswap == 0)
		handle_64(ofile);
	else if (ofile->is32 == 1 && ofile->isswap == 0)
		handle_32(ofile);
		//ft_printf("is32");
	else if (ofile->isswap == 1 && ofile->is32 == 0)
		handle_64(ofile);
	else if (ofile->isswap == 1 && ofile->is32 == 1)
		handle_32(ofile);
	(void) arg;
}

void nmfat(t_ofile *ofile, t_argvise *arg, int i)
{
	struct fat_header	*header;
	struct fat_arch		*arch;
	unsigned int		iarch;
	cpu_type_t 			cpu;

	iarch = 0;
	header = ofile->ptr;

	while (iarch < ofile->isfat)
	{
		arch = (ofile->fatptr + sizeof(struct fat_header) + (sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		ofile->arch = swap32(arch->cputype);
		if (ofile->isfatswap)
			cpu = swap32(arch->cputype);
		else
			cpu = arch->cputype;
		if (cpu == CPU_TYPE_X86_64)
		{
			nm(ofile, arg, i, 0);
			return ;
		}
		iarch++;
	}

	iarch = 0;
	while (iarch < ofile->isfat)
	{
		//ft_printf("%s \n", ofile->path);
		arch = (ofile->fatptr + sizeof(struct fat_header) + (sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		//ofile->arch = swap32(arch->cputype);
		ft_printf("\n");
		if (ofile->isfatswap)
		{
			ft_printf("%s ", arg->files[i]);
			show_archtypefor(swap32(arch->cputype));
		}
		else
		{
			ft_printf("%s", arg->files[i]);
			show_archtypefor(arch->cputype);
		}
		ft_printf(":\n");
		nm(ofile, arg, i, 0);
		iarch++;
	}
}

void ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	checkfat(ofile);
 	if (ofile->isfat)
		nmfat(ofile, arg, i);
	else
	{
		//ft_printf("%s", ofile->path);
		nm(ofile, arg, i, 1);
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
		ft_dprintf(2, "Please give me argv");
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
		//else
			//return (EXIT_FAILURE);
		i++;
	}
	return (0);
}
