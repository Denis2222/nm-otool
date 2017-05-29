/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 14:48:58 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 15:03:56 by dmoureu-         ###   ########.fr       */
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
	symbols = (void*)ofile->ptr + toswap32(ofile, sym->symoff);
	strtable = (void*)ofile->ptr + toswap32(ofile, sym->stroff);
	liste = NULL;
	while (i < toswap32(ofile, sym->nsyms))
	{
		if (ft_strlen(strtable + toswap32(ofile, symbols[i].n_un.n_strx)) > 0)
		{
			liste = addsymtabsort(&liste, newsymtab(sym, strtable +
				toswap32(ofile, symbols[i].n_un.n_strx), ofile->ptr, i));
		}
		i++;
	}
	showsymtabs(liste, ofile);
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
		if (toswap32(ofile, lc->cmd) == LC_SYMTAB)
		{
			print_output_32((struct symtab_command *)lc, ofile);
			break ;
		}
		lc = (void *)lc + toswap32(ofile, lc->cmdsize);
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
	symbols = (void*)ofile->ptr + sym->symoff;
	strtable = (void*)ofile->ptr + sym->stroff;
	liste = NULL;
	while (i < sym->nsyms)
	{
		if (ft_strlen(strtable + symbols[i].n_un.n_strx) > 0)
			liste = addsymtabsort(&liste, newsymtab(sym,
				strtable + symbols[i].n_un.n_strx, ofile->ptr, i));
		i++;
	}
	showsymtabs(liste, ofile);
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
		if (lc->cmd == LC_SYMTAB)
		{
			print_output_64((struct symtab_command *)lc, ofile);
			break ;
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}
