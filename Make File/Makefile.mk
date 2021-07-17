a.out:main.o getname.o
	gcc main.o getname.o

main.o:main.c
	gcc -c main.c

getname.o:getname.c
	gcc -c getname.c