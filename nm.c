#include <stdio.h>
#include <sys/mman.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/ranlib.h>
#include <mach-o/fat.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <xlocale.h>

void	print_output(struct symtab_command *sym, char *ptr)
{
	char			*strtable;
	struct nlist_64	*symbols;
	unsigned int	i;
	unsigned char	c;

	i = 0;
	symbols = (void*)ptr+sym->symoff; // Symbol table start location
	strtable = (void*)ptr+sym->stroff; // Location of the string table
	//printf("\n============%#x=================\nOutput symtab_command:\n", array[i].n_type);
	while (i<sym->nsyms)
	{
		c = symbols[i].n_type;

		switch(c & N_TYPE)
		{
			case N_UNDF:
				c = 'u';
				if(symbols[i].n_value != 0)
					c = 'c';
			break;
			case N_PBUD:
				c = 'u';
			break;
			case N_ABS:
				c = 'a';
			break;
			case N_SECT:
				//if(symbols[i].n_sect == process_flags->text_nsect)
					c = 't';
				/*else if(symbols[i].n_sect == process_flags->data_nsect)
					c = 'd';
				else if(symbols[i].n_sect == process_flags->bss_nsect)
					c = 'b';
				else
					c = 's';*/
			break;
			case N_INDR:
				c = 'i';
			break;
			default:
				c = '?';
			break;
		}
		if((symbols[i].n_type & N_EXT) && c != '?')
				c = toupper(c);
		printf("===========%c================", c);

		//printf("name:%s sym->cmd:%d  n_type:%#x n_value:%0.16llx \n", strtable + array[i].n_un.n_strx, sym->cmd, (array[i].n_type & N_TYPE), array[i].n_value);
		if ((symbols[i].n_type & N_TYPE) == N_UNDF)
			printf("                 U %s  \n", strtable + symbols[i].n_un.n_strx);
		else if ((symbols[i].n_type & N_TYPE) == N_SECT)
			printf("%0.16llx T %s\n", symbols[i].n_value, strtable + symbols[i].n_un.n_strx);
		i++;
	}
}

void show_filetype(unsigned int filetype)
{
	printf("MACH HEADER FILETYPE: ");
	if (filetype == MH_OBJECT)
		printf("MH_OBJECT");
	if (filetype == MH_EXECUTE)
		printf("MH_EXECUTE");
	if (filetype == MH_BUNDLE)
		printf("MH_BUNDLE");
	if (filetype == MH_DYLIB)
		printf("MH_DYLIB");
	if (filetype == MH_PRELOAD)
		printf("MH_PRELOAD");
	if (filetype == MH_CORE)
		printf("MH_CORE");
	if (filetype == MH_DYLINKER)
		printf("MH_DYLINKER");
	if (filetype == MH_DSYM)
		printf("MH_DSYM");
	printf("\n");
}

void show_load_command_type(struct load_command *lc)
{
	printf("LOAD_COMMAND TYPE:");
	if (lc->cmd == LC_UUID)
		printf("LC_UUID");
	else if (lc->cmd == LC_SEGMENT)
		printf("LC_SEGMENT");
	else if (lc->cmd == LC_SEGMENT_64)
		printf("LC_SEGMENT_64");
	else if (lc->cmd == LC_SYMTAB)
		printf("LC_SYMTAB");
	else if (lc->cmd == LC_DYSYMTAB)
		printf("LC_DYSYMTAB");
	else if (lc->cmd == LC_THREAD)
		printf("LC_THREAD");
	else if (lc->cmd == LC_UNIXTHREAD)
		printf("LC_UNIXTHREAD");
	else if (lc->cmd == LC_LOAD_DYLIB)
		printf("LC_LOAD_DYLIB");
	else if (lc->cmd == LC_ID_DYLIB)
		printf("LC_ID_DYLIB");
	else if (lc->cmd == LC_PREBOUND_DYLIB)
		printf("LC_PREBOUND_DYLIB");
	else if (lc->cmd == LC_LOAD_DYLINKER)
		printf("LC_LOAD_DYLINKER");
	else if (lc->cmd == LC_ID_DYLINKER)
		printf("LC_ID_DYLINKER");
	else if (lc->cmd == LC_ROUTINES)
		printf("LC_ROUTINES");
	else if (lc->cmd == LC_ROUTINES_64)
		printf("LC_ROUTINES_64");
	else if (lc->cmd == LC_TWOLEVEL_HINTS)
		printf("LC_TWOLEVEL_HINTS");
	else if (lc->cmd == LC_SUB_UMBRELLA)
		printf("LC_SUB_UMBRELLA");
	else if (lc->cmd == LC_SUB_LIBRARY)
		printf("LC_SUB_LIBRARY");
	else if (lc->cmd == LC_SUB_CLIENT)
		printf("LC_SUB_CLIENT");
else if (lc->cmd == LC_VERSION_MIN_MACOSX)
	printf("LC_VERSION_MIN_MACOSX");
	else
		printf("Unknow LC: %u", lc->cmd);
	printf("\n");
}

void	handle_64(char *ptr)
{
	unsigned int			i;
	struct mach_header_64	*mh;
	struct load_command		*lc;
	struct symtab_command	*sym;

	mh = (struct mach_header_64 *) ptr;
	//printf("==================\ninfo mach_header:\ncputype:%d\nfiletype:%d\n",mh->cputype, mh->filetype);
	show_filetype(mh->filetype);
	i = 0;
	lc = (void*)(ptr + sizeof(*mh));
	while (i < mh->ncmds)
	{
		show_load_command_type(lc);
		if (lc->cmd == LC_SYMTAB)
		{
			sym = (struct symtab_command *)lc;
			print_output(sym, ptr);
			break;
		}
		else
		{
			//printf("]]]]]]]]]]]]]]]load_command->cmd:%#x\n", lc->cmd);
		}
		lc = (void *) lc + lc->cmdsize;
		i++;
	}
}

void nm(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(unsigned int *) ptr;
	//printf("MAGIC_NUMBER get:%#x            REAL MH_MAGIC_64:%#x  FAT_MAGIC:%#x\n", magic_number, MH_MAGIC_64, FAT_MAGIC);
	if (magic_number == MH_MAGIC_64)
	{
		handle_64(ptr);
		puts("Je suis un binaire 64 bits");
	}
	else if (magic_number == FAT_MAGIC)
	{
		printf("FAT_MAGIC");
	}
	else
	{
		printf("%#x", magic_number);
	}
}

int main(int ac, char **av)
{
	int fd;
	char *ptr;
	struct stat buf;

	if (ac != 2)
	{
		fprintf(stderr, "Please give me argv");
		return (EXIT_FAILURE);
	}
	if ((fd = open(av[1], O_RDONLY)) < 0)
	{
		perror("open");
		return (EXIT_FAILURE);
	}
	if (fstat(fd, &buf) < 0)
	{
		perror("fstat");
		return (EXIT_FAILURE);
	}
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	{
		perror("mmap");
		return (EXIT_FAILURE);
	}
	nm(ptr);
	if (munmap(ptr, buf.st_size) < 0)
	{
		perror("munmap");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
