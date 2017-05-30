/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   archtype.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/29 10:39:13 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 19:58:31 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nmotool.h"

void	show_archtype(cpu_type_t arch)
{
	if (arch == CPU_TYPE_X86_64)
		ft_printf("(architecture x86_64)");
	else if (arch == CPU_TYPE_X86 || arch == CPU_TYPE_I386)
		ft_printf("(architecture i386)");
	else if (arch == CPU_TYPE_POWERPC)
		ft_printf("(architecture ppc)");
	else if (arch == CPU_TYPE_POWERPC64)
		ft_printf("(architecture ppc64)");
	else if (arch == CPU_TYPE_ARM)
		ft_printf("(architecture arm)");
	else if (arch == CPU_TYPE_ARM64)
		ft_printf("(architecture arm64)");
	else
		ft_printf("(specific architecture)");
}

void	show_archtypefor(cpu_type_t arch)
{
	if (arch == CPU_TYPE_X86_64)
		ft_printf("(for architecture x86_64)");
	else if (arch == CPU_TYPE_X86 || arch == CPU_TYPE_I386)
		ft_printf("(for architecture i386)");
	else if (arch == CPU_TYPE_POWERPC)
		ft_printf("(for architecture ppc)");
	else if (arch == CPU_TYPE_POWERPC64)
		ft_printf("(for architecture ppc64)");
	else if (arch == CPU_TYPE_ARM)
		ft_printf("(for architecture arm)");
	else if (arch == CPU_TYPE_ARM64)
		ft_printf("(for architecture arm64)");
	else
		ft_printf("(for specific architecture)");
}
