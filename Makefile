Ncurses-final-project : file1.o idncrc.o 
	gcc idncrc.o file1.o -o project
file1.o : file1.c
	gcc -c file1.c
idncrc.o : idncrc.c
		gcc -c idncrc.c


