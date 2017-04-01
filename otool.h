/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:56:48 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/04/01 06:25:41 by dmoureu-         ###   ########.fr       */
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

#include "libft/libft.h"
