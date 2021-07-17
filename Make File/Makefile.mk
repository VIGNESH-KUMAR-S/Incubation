OUTPUT = output.exe

#Executable File Creations
a.out:main.o add.o
	gcc main.o add.o -o output

#Library File Creation
library:
	lib add.o

#Object File Creation
main.o:main.c
	gcc -c main.c

add.o:add.c
	gcc -c add.c

clean:
    del *.o
	del *.lib
	del $(OUTPUT)