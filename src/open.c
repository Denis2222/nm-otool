/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/17 13:49:12 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/05/29 10:42:14 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../nmotool.h"

t_ofile	*mmap_ofile(int fd, t_ofile *ofile, char *path)
{
	if (ofile->buf.st_size < (long long)sizeof(struct mach_header))
	{
		ft_dprintf(2, "Invalid file format %s\n", path);
		return (NULL);
	}
	if ((ofile->ptr = mmap(0, ofile->buf.st_size, PROT_READ,
		MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "mmap fail on %s", path);
		return (NULL);
	}
	return (ofile);
}

t_ofile	*open_ofile(char *path)
{
	int			fd;
	t_ofile		*ofile;

	ofile = (t_ofile*)malloc(sizeof(t_ofile));
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "open fail on %s", path);
		return (NULL);
	}
	ofile->path = path;
	if (fstat(fd, &(ofile->buf)) < 0)
	{
		ft_dprintf(2, "fstat fail on %s", path);
		return (NULL);
	}
	return (mmap_ofile(fd, ofile, path));
}

void	close_ofile(t_ofile *ofile)
{
	if (munmap(ofile->ptr, ofile->buf.st_size) < 0)
	{
		ft_dprintf(2, "munmap fail on %s", ofile->path);
		return ;
	}
}
