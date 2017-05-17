/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:02 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 15:33:18 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nm-otool.h"

typedef struct				s_symtab
{
	struct symtab_command	*sym;
	char					*name;
	char					*ptr;
	unsigned int			i;
	struct s_symtab			*next;
}							t_symtab;

t_symtab					*newsymtab(struct symtab_command *sym, char *name, char *ptr, unsigned int i);
t_symtab					*addsymtabsort(t_symtab **liste, t_symtab *new);
void						showsymtab(t_symtab *s);
void						showsymtabs(t_symtab *liste);
