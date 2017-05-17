/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 14:30:53 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/17 14:31:07 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../otool.h"

void	ft_puthex(unsigned char c)
{
	char tab[16] = "0123456789abcdef";

	ft_putchar(tab[c / 16]);
	ft_putchar(tab[c % 16]);
}

void	print_line(unsigned char *str, size_t start, size_t max, int isppc)
{
	size_t i;

	i = start;
	while (i < start + 16 && i < max)
	{
		(void)isppc;
		if (isppc)
		{
			ft_puthex(str[i]);
			ft_puthex(str[i+1]);
			ft_puthex(str[i+2]);
			ft_puthex(str[i+3]);
			ft_putchar(' ');
			i = i + 4;
		}
		else
		{
			ft_puthex(str[i++]);
			ft_putchar(' ');
		}
	}
	ft_putchar('\n');
}
