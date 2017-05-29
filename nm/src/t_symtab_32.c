/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symtab_32.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 14:52:03 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 16:13:56 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

char	typeofse_32sub(t_ofile *o, struct load_command *lc, int *g, int n_sect)
{
	struct segment_command	*sc;
	struct section			*se;
	int						i;

	sc = (struct segment_command *)lc;
	i = 0;
	se = (void *)sc + sizeof(struct segment_command);
	while ((uint32_t)i < toswap32(o, sc->nsects))
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

char	findtypeofsection_32(int n_sect, t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header		*mh;
	struct load_command		*lc;
	int						g;
	char					c;

	mh = (struct mach_header *)ofile->ptr;
	i = 0;
	g = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < toswap32(ofile, mh->ncmds))
	{
		if (toswap32(ofile, lc->cmd) == LC_SEGMENT)
			if ((c = typeofse_32sub(ofile, lc, &g, n_sect)))
				return (c);
		lc = (void *)lc + toswap32(ofile, lc->cmdsize);
		i++;
	}
	return ('s');
}

char	showtype_32(t_ofile *ofile, struct nlist *symbol)
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
		c = findtypeofsection_32(symbol->n_sect, ofile);
	else if ((symbol->n_type & N_TYPE) == N_SECT)
		c = 'i';
	else if ((symbol->n_type & N_EXT) && !(symbol->n_type & N_TYPE))
		c = 'a';
	else
		c = '?';
	return (c);
}

void	showsymtab_32(t_symtab *s, t_ofile *ofile)
{
	char			*strtable;
	struct nlist	*symbols;
	unsigned int	i;
	char			c;

	i = s->i;
	symbols = (void*)s->ptr + toswap32(ofile, s->sym->symoff);
	strtable = (void*)s->ptr + toswap32(ofile, s->sym->stroff);
	c = showtype_32(ofile, &symbols[i]);
	if ((symbols[i].n_type & N_EXT) && c != '?')
		c = ft_toupper(c);
	if (c != 'u')
	{
		if ((symbols[i].n_type & N_TYPE) == N_UNDF)
			ft_printf("         %c %s\n", c, s->name);
		else if (((symbols[i].n_type & N_TYPE) == N_SECT) || c == 'A')
			ft_printf("%0.8x %c %s\n",
				toswap32(ofile, symbols[i].n_value), c, s->name);
	}
}
