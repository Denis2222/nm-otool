/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:54:42 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 17:47:00 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../otool.h"

void	otool(t_ofile *ofile, t_argvise *arg, int i, int show)
{
	struct mach_header *test;

	if (!checktype(ofile))
	{
		ft_dprintf(2,
		"%s: %s: The file was not recognized as a valid object file.\n",
		arg->argv[0], arg->files[i]);
		return ;
	}
	test = (void *)ofile->ptr;
	if (show)
		ft_printf("%s:", arg->files[i]);
	if (ofile->is32 == 0 && ofile->isswap == 0)
		handle_64(ofile);
	else if (ofile->is32 == 1)
		handle_32(ofile);
	(void)arg;
}

int		main(int argc, char **argv)
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
		else
			return (EXIT_FAILURE);
		i++;
	}
	return (0);
}

int		otoolfat_special(t_ofile *ofile, int i, t_argvise *arg)
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
		if ((ofile->isfatswap && swap32(arch->cputype) == CPU_TYPE_X86_64)
			|| arch->cputype == CPU_TYPE_X86_64)
		{
			otool(ofile, arg, i, 1);
			return (1);
		}
		iarch++;
	}
	return (0);
}

void	otoolfat(t_ofile *ofile, t_argvise *arg, int i)
{
	struct fat_arch		*arch;
	unsigned int		iarch;

	iarch = 0;
	if (otoolfat_special(ofile, i, arg))
		return ;
	while (iarch < ofile->isfat)
	{
		arch = (ofile->fatptr + sizeof(struct fat_header) +
			(sizeof(struct fat_arch) * iarch));
		if (ofile->isfatswap)
			ofile->ptr = ofile->fatptr + swap32(arch->offset);
		else
			ofile->ptr = ofile->fatptr + arch->offset;
		ft_printf("%s ", arg->files[i]);
		if (ofile->isfatswap)
			show_archtype(swap32(arch->cputype));
		else
			show_archtype(arch->cputype);
		ft_printf(":");
		otool(ofile, arg, i, 0);
		iarch++;
	}
}

void	ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	(void)i;
	checkfat(ofile);
	if (ofile->isfat)
		otoolfat(ofile, arg, i);
	else
	{
		otool(ofile, arg, i, 1);
	}
}
