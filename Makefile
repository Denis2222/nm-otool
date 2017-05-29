.PHONY:nm otool

all: nm otool

nm:
	make -C ./nm/
	cp ./nm/ft_nm ./

otool:
	make -C ./otool/
	cp ./otool/ft_otool ./
clean:
	make -C ./nm/ clean
	make -C ./otool/ clean
	rm ./ft_nm
	rm ./ft_otool

fclean: clean
	make -C ./nm/ fclean
	make -C ./otool/ fclean
	
re: fclean all
