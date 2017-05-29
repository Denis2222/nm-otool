/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nmotool.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 10:13:28 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 10:19:27 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NMOTOOL_H
# define NMOTOOL_H

# include <stdio.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/ranlib.h>
# include <mach-o/fat.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdlib.h>
# include <unistd.h>
# include "./libft/libft.h"

typedef struct		s_ofile
{
	void			*fatptr;
	void			*ptr;
	struct stat		buf;
	char			*path;
	unsigned int	isfat;
	unsigned int	isfatswap;
	unsigned int	isswap;
	unsigned int	is32;
	unsigned int	arch;
}					t_ofile;

uint32_t			swap32(uint32_t in);
uint32_t			toswap32(t_ofile *ofile, uint32_t var);
uint64_t			swap64(uint64_t in);
uint64_t			toswap64(t_ofile *ofile, uint64_t var);
void				checkfat(t_ofile *ofile);
int					checktype(t_ofile *ofile);
void				show_archtype(cpu_type_t arch);
void				show_archtypefor(cpu_type_t arch);
t_ofile				*open_ofile(char *path);
void				close_ofile(t_ofile *ofile);

#endif
