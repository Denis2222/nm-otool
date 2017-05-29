/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symtab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 15:31:34 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 09:23:10 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

t_symtab *newsymtab(struct symtab_command *sym, char *name, char *ptr, unsigned int i)
{
	t_symtab *new;

	new = (t_symtab*)malloc(sizeof(t_symtab)+1);
	new->sym = sym;
	//ft_printf(" $$$$$$$$$$$$$ %s ", name);
	new->name = ft_strdup(name);
	new->ptr = ptr;
	new->i = i;
	new->next = NULL;
	return (new);
}

t_symtab *addsymtabsort(t_symtab **liste, t_symtab *new)
{
	t_symtab *begin;
	t_symtab *current;
	t_symtab *tmp;

	//ft_printf("addsymtab %s ", new->name);
	begin = *liste;
	current = *liste;
	if (current)
	{
		//ft_printf(" %s %s ", current->name, new->name);
		if (ft_strcmp(current->name, new->name) > 0) //New < 1st
		{
			begin  = new;
			begin->next = current;
			return (begin);
		}
		while (current->next)
		{
			if (ft_strcmp(current->next->name, new->name) > 0) // New < next
			{
				tmp = current->next;
				current->next = new;
				new->next = tmp;
				return (begin);
			}
			current = current->next;
		}
		current->next = new;
	} else
		begin = new;
	return (begin);
}

char findtypeofsection_64(int n_sect, t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;
	int g;
	mh = (struct mach_header_64 *) ofile->ptr;
	i = 0;
	g = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
				struct segment_command_64 *sc;
				sc = (struct segment_command_64 *)lc;
				struct section_64 *se;
				int		i;

				i = 0;
				se = (void *)sc + sizeof(struct segment_command_64);
				while ((uint32_t)i < sc->nsects)
				{
					g++;
					//printf("%s %s\n", "sectname", se->sectname);
					if (g == n_sect)
					{
						//ft_printf("[%s %s %d t]\n", se->sectname, se->segname, g);
						if (ft_strcmp(se->sectname, "__text") == 0)
							return ('t');
						else if(ft_strcmp(se->sectname, "__data") == 0)
							return ('d');
						else if(ft_strcmp(se->sectname, "__bss") == 0)
							return ('b');
						else
							return ('s');
					}
					se++;
					i++;
				}
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
	return ('s');
}

char findtypeofsection_32(int n_sect, t_ofile *ofile)
{
	unsigned int			i;
	struct mach_header	*mh;
	struct load_command		*lc;
	int g;
	mh = (struct mach_header *) ofile->ptr;
	i = 0;
	g = 0;
	lc = (void*)(ofile->ptr + sizeof(*mh));
	while (i < toswap32(ofile,mh->ncmds))
	{
		if (toswap32(ofile,lc->cmd) == LC_SEGMENT)
		{
				struct segment_command *sc;
				sc = (struct segment_command *)lc;
				struct section *se;
				int		i;

				i = 0;
				se = (void *)sc + sizeof(struct segment_command);
				while ((uint32_t)i < toswap32(ofile,sc->nsects))
				{
					g++;
					//printf("%s %s\n", "sectname", se->sectname);
					if (g == n_sect)
					{
						//ft_printf("[%s %s %d t]\n", se->sectname, se->segname, g);
						if (ft_strcmp(se->sectname, "__text") == 0)
							return ('t');
						else if(ft_strcmp(se->sectname, "__data") == 0)
							return ('d');
						else if(ft_strcmp(se->sectname, "__bss") == 0)
							return ('b');
						else
							return ('s');
					}
					se++;
					i++;
				}
		}
		lc = (void *) lc + toswap32(ofile, lc->cmdsize);
		i++;
	}
	return ('s');
}


void showsymtab_32(t_symtab *s, t_ofile *ofile)
{
	char			*strtable;
	struct nlist	*symbols;
	unsigned int 	i;
	char			c;

	(void)ofile;


	i = s->i;
	symbols = (void*)s->ptr+toswap32(ofile,s->sym->symoff); // Symbol table start location
	strtable = (void*)s->ptr+toswap32(ofile,s->sym->stroff); // Location of the string table
	//printf("\n============%#x=================\nOutput symtab_command:\n", array[i].n_type);

		//ft_printf("Before explosion %d\n", i);
		c = symbols[i].n_type;

		switch(c & N_TYPE)
		{
			case N_UNDF:
				c = 'u';
				if(toswap32(ofile, symbols[i].n_value) != 0)
					c = 'c';
			break;
			case N_PBUD:
				c = 'u';
			break;
			case N_ABS:
				c = 'a';
			break;
			case N_SECT:
				c = findtypeofsection_32(symbols[i].n_sect, ofile);
			break;
			case N_INDR:
				c = 'i';
			break;
			default:
				c = '?';
			break;
		}

		if ((c & N_EXT) && !(c & N_TYPE))
			c = 'a';
		//if(strcmp(s->name, "__mh_execute_header") == 0)
			//ft_printf("SPOTTED N_TYPE:%d  N_EXT:%d  %c !! ", c & N_TYPE, c & N_EXT, c);
		if((symbols[i].n_type & N_EXT) && c != '?')
				c = ft_toupper(c);
		//printf(" %c ", c);

		//printf("name:%s sym->cmd:%d  n_type:%#x n_value:%0.16llx \n", strtable + array[i].n_un.n_strx, sym->cmd, (array[i].n_type & N_TYPE), array[i].n_value);

		if (c != 'u')
		{
			if ((symbols[i].n_type & N_TYPE) == N_UNDF)
				ft_printf("         %c %s\n", c, s->name);
			else if ((symbols[i].n_type & N_TYPE) == N_SECT)
				ft_printf("%0.8x %c %s\n", toswap32(ofile,symbols[i].n_value), c, s->name);//, strtable + symbols[i].n_un.n_strx
			else if (c == 'A')
				ft_printf("%0.8x %c %s\n", toswap32(ofile,symbols[i].n_value), c, s->name);//, strtable + symbols[i].n_un.n_strx
		}
}

void showsymtab_64(t_symtab *s, t_ofile *ofile)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int 	i;
	char			c;

	(void)ofile;
	i = s->i;
	symbols = (void*)s->ptr+toswap32(ofile,s->sym->symoff); // Symbol table start location
	strtable = (void*)s->ptr+toswap32(ofile,s->sym->stroff); // Location of the string table

		c = symbols[i].n_type;

		switch(c & N_TYPE)
		{
			case N_UNDF:
				c = 'u';
				if(toswap32(ofile, symbols[i].n_value) != 0)
					c = 'c';
			break;
			case N_PBUD:
				c = 'u';
			break;
			case N_ABS:
				c = 'a';
			break;
			case N_SECT:
				c = findtypeofsection_64(symbols[i].n_sect, ofile);
			break;
			case N_INDR:
				c = 'i';
			break;
			default:
				c = '?';
			break;
		}

		if ((c & N_EXT) && !(c & N_TYPE))
			c = 'a';

		if((symbols[i].n_type & N_EXT) && c != '?')
			c = ft_toupper(c);
		//ft_printf("%s   :   ", s->name);
		if (c != 'u' && c != '?')
		{
			if ((toswap32(ofile,symbols[i].n_type) & N_TYPE) == N_UNDF)
				ft_printf("                 %c %s\n", c, strtable + symbols[i].n_un.n_strx);
			else if ((toswap32(ofile,symbols[i].n_type) & N_TYPE) == N_SECT)
				ft_printf("%0.16llx %c %s\n", symbols[i].n_value, c, strtable + symbols[i].n_un.n_strx);
			else if (c == 'A')
				ft_printf("%0.16llx %c %s\n", toswap64(ofile, symbols[i].n_value), c, s->name);//, strtable + symbols[i].n_un.n_strx
			else
				ft_printf("%0.16llx %c %s\n", toswap64(ofile, symbols[i].n_value), c, s->name);
		}
}

void showsymtabs(t_symtab *liste, t_ofile *ofile)
{
	t_symtab *begin;
	t_symtab *current;

	begin = liste;
	current = liste;
	while (current)
	{
		//ft_printf("sst:%s\n", current->name);
		if (!ofile->is32)
			showsymtab_64(current, ofile);
		else
			showsymtab_32(current, ofile);

		//if (current->i == 76)
			//ft_printf("next-> %s", current->name);
		current = current->next;
	}
}
