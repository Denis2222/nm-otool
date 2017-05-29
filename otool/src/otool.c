/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:54:42 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 15:39:31 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../otool.h"

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
		otool(ofile, arg);
		iarch++;
	}
}

void ofileheader(t_ofile *ofile, t_argvise *arg, int i)
{
	(void) i;
	checkfat(ofile);
 	if (ofile->isfat)
		otoolfat(ofile, arg);
	else
	{
		ft_printf("%s", ofile->path);
		otool(ofile, arg);
	}
}
