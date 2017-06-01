/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symtab_64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 14:51:51 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/06/01 07:17:22 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

char	findtypeofsection_64sub(struct load_command *lc, int *g, int n_sect)
{
	struct segment_command_64	*sc;
	struct section_64			*se;
	int							i;

	sc = (struct segment_command_64 *)lc;
	i = 0;
	se = (void *)sc + sizeof(struct segment_command_64);
	while ((uint32_t)i < sc->nsects)
	{
		(*g)++;
		if ((*g) == n_sect)
		{
			if (ft_strcmp(se->sectname, "__text") == 0)
				return ('t');
			else if (ft_strcmp(se->sectname, "__data") == 0)
				return ('d');
			else if (ft_strcmp(se->sectname, "__bss") == 0)
				return ('b');
			else
				return ('s');
		}
		se++;
		i++;
	}
	return (0);
}

char	findtypeofsection_64(int n_sect, t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;
	int						g;
	char					c;

	mh = (struct mach_header_64 *)ofile->ptr;
	i = 0;
	g = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			if ((c = findtypeofsection_64sub(lc, &g, n_sect)))
				return (c);
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	return ('s');
}

char	showtype_64(t_ofile *ofile, struct nlist_64 *symbol)
{
	char c;

	c = 'u';
	if ((symbol->n_type & N_TYPE) == N_UNDF)
	{
		if (toswap32(ofile, symbol->n_value) != 0)
			c = 'c';
	}
	else if ((symbol->n_type & N_TYPE) == N_ABS)
		c = 'a';
	else if ((symbol->n_type & N_TYPE) == N_SECT)
		c = findtypeofsection_64(symbol->n_sect, ofile);
	else if ((symbol->n_type & N_TYPE) == N_INDR)
		c = 'i';
	else if ((symbol->n_type & N_EXT) && !(symbol->n_type & N_TYPE))
		c = 'a';
	else
		c = '?';
	return (c);
}

void	showsymtab_64(t_symtab *s, t_ofile *ofile)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	char			c;

	i = s->i;
	symbols = (void*)s->ptr + toswap32(ofile, s->sym->symoff);
	strtable = (void*)s->ptr + toswap32(ofile, s->sym->stroff);
	c = showtype_64(ofile, &symbols[i]);
	if ((symbols[i].n_type & N_EXT) && c != '?')
		c = ft_toupper(c);
	if (c != 'u' && c != '?')
	{
		if ((toswap32(ofile, symbols[i].n_type) & N_TYPE) == N_UNDF)
			ft_printf("                 %c %s\n", c, s->name);
		else
			ft_printf("%0.16llx %c %s\n",
				toswap64(ofile, symbols[i].n_value), c, s->name);
	}
}
