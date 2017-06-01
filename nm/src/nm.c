/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:35 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/06/01 06:33:47 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

void	nm(t_ofile *ofile, t_argvise *arg, int i, int show)
{
	if (!checktype(ofile))
	{
		ft_dprintf(2,
			"%s: %s: The file was not recognized as a valid object file.\n",
			arg->argv[0], arg->files[i]);
		return ;
	}
	if (arg->nfiles > 1 && show)
		ft_printf("\n%s:\n", arg->files[i]);
	if (ofile->is32 == 0 && ofile->isswap == 0)
		handle_64(ofile);
	else if (ofile->is32 == 1 && ofile->isswap == 0)
		handle_32(ofile);
	else if (ofile->isswap == 1 && ofile->is32 == 0)
		handle_64(ofile);
	else if (ofile->isswap == 1 && ofile->is32 == 1)
		handle_32(ofile);
}

int		nmfat_special(t_ofile *ofile, int i, t_argvise *arg)
{
	struct fat_arch		*arch;
	struct fat_header	*header;
	unsigned int		iarch;

	iarch = 0;
	header = ofile->ptr;
	while (iarch < ofile->isfat)
	{
		arch = (ofile->fatptr + sizeof(struct fat_header) +
			(sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		ofile->arch = swap32(arch->cputype);
		if ((ofile->isfatswap && swap32(arch->cputype) == CPU_TYPE_X86_64)
			|| arch->cputype == CPU_TYPE_X86_64)
		{
			nm(ofile, arg, i, 0);
			return (1);
		}
		iarch++;
	}
	return (0);
}

void	nmfat(t_ofile *ofile, t_argvise *arg, int i)
{
	struct fat_arch		*arch;
	unsigned int		iarch;

	iarch = 0;
	if (nmfat_special(ofile, i, arg))
		return ;
	while (iarch < ofile->isfat)
	{
		arch = (ofile->fatptr + sizeof(struct fat_header) +
			(sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		ft_printf("\n%s ", arg->files[i]);
		if (ofile->isfatswap)
			show_archtypefor(swap32(arch->cputype));
		else
			show_archtypefor(arch->cputype);
		ft_printf(":\n");
		nm(ofile, arg, i, 0);
		iarch++;
	}
}

void	ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	checkfat(ofile);
	if (ofile->isfat)
		nmfat(ofile, arg, i);
	else
		nm(ofile, arg, i, 1);
}

int		main(int argc, char **argv)
{
	int			i;
	t_ofile		*ofile;
	t_argvise	*arg;

	arg = new_argvise(argc, argv);
	if (argc <= 1)
	{
		ft_dprintf(2, "Usage: nm 'mach-o exec'\n");
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
		i++;
	}
	return (0);
}
