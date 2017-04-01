/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmoureu- <dmoureu-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/01 00:54:42 by dmoureu-          #+#    #+#             */
/*   Updated: 2017/04/01 06:52:09 by dmoureu-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

typedef struct	s_ofile
{
	void		*ptr;
	struct stat	buf;
}				t_ofile;

t_ofile *open_ofile(char *path)
{
	int			fd;
	struct stat	buf;

	t_ofile		*ofile;
	ofile = (t_ofile*)malloc(sizeof(t_ofile));
	if ((fd = open(path, O_RDONLY)) < 0)
	{
		ft_dprintf(2, "open fail on %s", path);
		return (NULL);
	}
	if (fstat(fd, &buf) < 0)
	{
		ft_dprintf(2, "fstat fail on %s", path);
		return (NULL);
	}
	if ((ofile->ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		ft_dprintf(2, "mmap fail on %s", path);
		return (NULL);
	}

	return (ofile);
}

void	close_ofile(t_ofile *ofile)
{
	if (munmap(ofile->ptr, ofile->buf.st_size) < 0)
	{
		ft_dprintf(2, "munmap fail on %s", path);
		return (NULL);
	}
}

int			main(int argc, char **argv)
{
	int			i;
	t_ofile		*ofile;
	t_argvise	*arg;
	arg = new_argvise(argc, argv);

	if (argc <= 1)
	{
		fprintf(stderr, "Please give me argv");
		return (EXIT_FAILURE);
	}

	i = 0;
	while (arg->files[i] != '\0')
	{
		if (ofile = open_ofile(arg->files[i]))
		{
			ft_printf("file:%s %p\n", arg->files[i], ofile);
			close_ofile(ofile);
		}
		else
			return (EXIT_FAILURE);
		i++;
	}
	return (0);
}
