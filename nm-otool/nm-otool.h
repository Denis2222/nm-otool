/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm-otool.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 13:47:40 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 15:02:59 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#include "./libft/libft.h"

typedef struct	s_ofile
{
	void 			*fatptr;
	void			*ptr;
	struct stat		buf;
	char 			*path;
	unsigned int	isfat;
	unsigned int	isfatswap;
	unsigned int	isswap;
	unsigned int 	is32;
	unsigned int	arch;
}					t_ofile;

uint32_t 	swap32(uint32_t in);
uint32_t 	toswap32(t_ofile *ofile, uint32_t var);
void 		checkfat(t_ofile *ofile);
void 		checktype(t_ofile *ofile);
void 		show_archtype(cpu_type_t arch);
t_ofile		*open_ofile(char *path);
void		close_ofile(t_ofile *ofile);
