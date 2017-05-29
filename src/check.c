/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 10:40:37 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 10:42:20 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nmotool.h"

void	checkfat(t_ofile *ofile)
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

int		checktype(t_ofile *ofile)
{
	uint32_t	magic;

	magic = *(unsigned int *)ofile->ptr;
	if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
	{
		ofile->is32 = 0;
		ofile->isswap = 0;
		if (magic == MH_CIGAM_64)
			ofile->isswap = 1;
		return (1);
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		ofile->is32 = 1;
		ofile->isswap = 0;
		if (magic == MH_CIGAM)
			ofile->isswap = 1;
		return (1);
	}
	return (0);
}
