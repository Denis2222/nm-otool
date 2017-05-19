all: nm otool

nm:
	make -C ./nm/
	cp ./nm/ft_nm ./nm

otool:
	make -C ./otool/
	cp ./otool/ft_otool ./otool

clean:
	make -C ./nm/ clean
	make -C ./otool/ clean


fclean: clean
	make -C ./nm/ fclean
	make -C ./otool/ fclean

re: fclean all
