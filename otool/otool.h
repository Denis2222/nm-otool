/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:56:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 16:26:22 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OTOOL_H
# define OTOOL_H

# include "../nmotool.h"

void	print_memory_32(t_ofile *ofile, struct section *se);
void	print_section_32(t_ofile *ofile, struct segment_command *sc);
void	print_segment_32(struct load_command *lc, t_ofile *ofile);
void	handle_32(t_ofile *ofile);

void	print_memory_64(const void *addr, size_t size, uint64_t offset);
void	print_section_64(t_ofile *ofile, struct segment_command_64 *sc);
void	print_segment_64(struct load_command *lc, t_ofile *ofile);
void	handle_64(t_ofile *ofile);

void	print_line(unsigned char *str, size_t start, size_t max, int isppc);
void	ft_puthex(unsigned char c);

void	otool(t_ofile *ofile, t_argvise *arg, int i);
void	otoolfat(t_ofile *ofile, t_argvise *arg, int i);
void	ofileheader(t_ofile *ofile, t_argvise *arg, int i);

#endif
