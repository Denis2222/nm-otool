/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 10:40:02 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 10:42:17 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nmotool.h"

uint32_t	swap32(uint32_t in)
{
	uint32_t out;

	out = ((in & 0xFF) << 24) | ((in & 0xFF00) << 8) |
			((in >> 8) & 0xFF00) | ((in >> 24) & 0xFF);
	return (out);
}

uint32_t	toswap32(t_ofile *ofile, uint32_t var)
{
	if (ofile->isswap)
		return (swap32(var));
	else
		return (var);
}

uint64_t	swap64(uint64_t in)
{
	in = ((in << 8) & 0xFF00FF00FF00FF00ULL) |
			((in >> 8) & 0x00FF00FF00FF00FFULL);
	in = ((in << 16) & 0xFFFF0000FFFF0000ULL) |
			((in >> 16) & 0x0000FFFF0000FFFFULL);
	return (in << 32) | (in >> 32);
}

uint64_t	toswap64(t_ofile *ofile, uint64_t var)
{
	if (ofile->isswap)
		return (swap64(var));
	else
		return (var);
}
