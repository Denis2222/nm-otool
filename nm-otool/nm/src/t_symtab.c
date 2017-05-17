/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symtab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 15:31:34 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 15:36:04 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

t_symtab *newsymtab(struct symtab_command *sym, char *name, char *ptr, unsigned int i)
{
	t_symtab *new;

	new = (t_symtab*)malloc(sizeof(t_symtab)+1);
	new->sym = sym;
	new->name = strdup(name);
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

	begin = *liste;
	current = *liste;
	if (current)
	{
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

void showsymtab(t_symtab *s)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int 	i;
	char			c;
	i = s->i;
	symbols = (void*)s->ptr+s->sym->symoff; // Symbol table start location
	strtable = (void*)s->ptr+s->sym->stroff; // Location of the string table
	//printf("\n============%#x=================\nOutput symtab_command:\n", array[i].n_type);

		c = symbols[i].n_type;

		switch(c & N_TYPE)
		{
			case N_UNDF:
				c = 'u';
				if(symbols[i].n_value != 0)
					c = 'c';
			break;
			case N_PBUD:
				c = 'u';
			break;
			case N_ABS:
				c = 'a';
			break;
			case N_SECT:
				//findtypeofsection(symbols[i].n_sect, ptr);
				if(symbols[i].n_sect == 1)
					c = 't';
				else if(symbols[i].n_sect == 2)
					c = 'd';
				else if(symbols[i].n_sect == 3)
					c = 'b';
				else
					c = 's';
			break;
			case N_INDR:
				c = 'i';
			break;
			default:
				c = '?';
			break;
		}
		if((symbols[i].n_type & N_EXT) && c != '?')
				c = toupper(c);
		//printf(" %c ", c);

		//printf("name:%s sym->cmd:%d  n_type:%#x n_value:%0.16llx \n", strtable + array[i].n_un.n_strx, sym->cmd, (array[i].n_type & N_TYPE), array[i].n_value);
		if ((symbols[i].n_type & N_TYPE) == N_UNDF)
			printf("                 %c %s  \n", c, strtable + symbols[i].n_un.n_strx);
		else if ((symbols[i].n_type & N_TYPE) == N_SECT)
			printf("%0.16llx %c %s\n", symbols[i].n_value, c, strtable + symbols[i].n_un.n_strx);

}

void showsymtabs(t_symtab *liste)
{
	t_symtab *begin;
	t_symtab *current;

	begin = liste;
	current = liste;

		while (current)
		{
			showsymtab(current);
			current = current->next;
		}
}
