/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:46:02 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 16:13:53 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "../nmotool.h"

typedef struct				s_symtab
{
	struct symtab_command	*sym;
	char					*name;
	char					*ptr;
	unsigned int			i;
	struct s_symtab			*next;
}							t_symtab;

t_symtab					*newsymtab(struct symtab_command *sym, char *name,
							char *ptr, unsigned int i);
t_symtab					*addsymtabsort(t_symtab **liste, t_symtab *new);
void						showsymtabs(t_symtab *liste, t_ofile *ofile);
void						showsymtab(t_symtab *s, t_ofile *ofile);
void						showsymtab_64(t_symtab *s, t_ofile *ofile);
void						showsymtab_32(t_symtab *s, t_ofile *ofile);
void						handle_64(t_ofile *ofile);
void						handle_32(t_ofile *ofile);

#endif
