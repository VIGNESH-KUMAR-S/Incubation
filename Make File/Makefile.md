# Make file Concept
- Makefile is a tool to simplify or to organize code for compilation. 
- Consider, we have a multiple source files that connected to each other. If we want to run main function then we should run all the available source files every time.
- Since, command is very big for projects.
- By using Makefile concept, all the commands are written in **Makefile**.
- If we want to change the contents of one source file then we should re-compile that source file otherwise results are mismatched. In this scenario there is no need to write all the commands. We just run the makefile.
- ```nmake /f Makefile.mk [NAME]```
- The above command is used to run the commands under particular **NAME**.
- Example :- **`Makefile.mk`**
```
    #Creation of Variable
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
```

- In the above example, If we want to run the commands under **`clean`**.
- `nmake /f Makefile.mk clean`
- Similarly for **`library`**
- `nmake /f Makefile.mk library`
- To run Makefile.mk 
- `nmake Makefile.mk`
