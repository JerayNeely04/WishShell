#make file for 2 source files : bar.c and foo.c


#Makes the main executable file

wish: main.o commands.o 
	gcc main.o commands.o -o wish

#compiles the main.c file with the foo.h and bar.h headers 
main.o: main.c commands.h
	gcc -c main.c


###################################

#complies foo.c 
commmands.o: prompt.c
	gcc -c commmands.c


zip:
	zip submission.zip *.c *.h Makefile

