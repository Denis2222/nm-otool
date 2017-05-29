/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   t_symtab.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 15:31:34 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 16:14:02 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm.h"

t_symtab	*newsymtab(struct symtab_command *sym, char *name,
	char *ptr, unsigned int i)
{
	t_symtab	*new;

	new = (t_symtab*)malloc(sizeof(t_symtab) + 1);
	new->sym = sym;
	new->name = ft_strdup(name);
	new->ptr = ptr;
	new->i = i;
	new->next = NULL;
	return (new);
}

t_symtab	*addsymtabsortplus(t_symtab **sym, t_symtab *new)
{
	if (ft_strcmp(sym[1]->name, new->name) > 0)
	{
		sym[0] = new;
		sym[0]->next = sym[1];
		return (sym[0]);
	}
	while (sym[1]->next)
	{
		if (ft_strcmp(sym[1]->next->name, new->name) > 0)
		{
			sym[2] = sym[1]->next;
			sym[1]->next = new;
			new->next = sym[2];
			return (sym[0]);
		}
		sym[1] = sym[1]->next;
	}
	return (0);
}

t_symtab	*addsymtabsort(t_symtab **liste, t_symtab *new)
{
	t_symtab	*sym[3];

	sym[0] = *liste;
	sym[1] = *liste;
	if (sym[1])
	{
		if ((sym[2] = addsymtabsortplus(sym, new)))
			return (sym[2]);
		sym[1]->next = new;
	}
	else
		sym[0] = new;
	return (sym[0]);
}

void		showsymtabs(t_symtab *liste, t_ofile *ofile)
{
	t_symtab	*begin;
	t_symtab	*current;

	begin = liste;
	current = liste;
	while (current)
	{
		if (!ofile->is32)
			showsymtab_64(current, ofile);
		else
			showsymtab_32(current, ofile);
		current = current->next;
	}
}
