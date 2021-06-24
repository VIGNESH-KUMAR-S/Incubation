# Library in C
- In general, library contains set of resources to enhance our skills.
- Also, In C language **library contains set of codes that may be a function, variable, etc,**.
- It helps us to avoid timings to write a program.

### Two types of libraries in C language
  1. Static Library
  2. Shared Library (Dynamic Library)

## Static Library
- Static library is a file contains a **collection of object files**, that are linked during linking phase.
- All machine codes are inserted into executable file.
- Extension is **`.a` in linux or `.lib` in windows**.
- This is done at compile time itself.

### Advantages and Dis-advantages
- In Static library, all object files are physically present inside the executable file.
- Only the executable file is loaded. Static library object files not loaded.
- The speed of execution at run-time occurs faster because its object code (binary) is in the executable file. Thus, calls made to the functions get executed quicker.
- Changes made to the files and program require relinking and recompilation.
- File size is much larger, because all object files present in single static library file.

### Commands
```
	cl /c calc.c	=>	/c flag for compile and assemble only but don't link
	lib calc.obj	=>	library file created
	cl main.c C:\Users\Vignesh\...[PATH OF LIBRARY]
```

## Dynamic Library
- Dynamic library is a file contains **collection of object files**, all object file's references are linked to the executable file.
- With dynamic libraries, machine code is only loaded and executed at runtime.
- At linking phase, all addresses of object files are stored in executable file.
- Where as at run time, the object code will be loaded from library via reference.

### Advantages and Dis-advantages
- In dynamic library, all object file's reference are present in executable file.
- Where as the object files are loaded from library via reference.
- The speed of execution time is less, because for every object file it'll be loading and un-loading.
- Re-Compilation not requires because of reference in executable file.
- File size is smaller only because reference of object file have less memory only.
- Library is deleted then loading that library not possible.


![](https://pussgrc.github.io/assets/img/blog/post03.jpg)

### Commands
```
	cl calcdll.c /LD	=>	/LD used to generate dll file and library file
	cl main.c C:\Users\Vignesh\...[PATH OF LIBRARY]
```

# Reference
1. [Static library](https://medium.com/@luischaparroc/c-static-library-bc9a050bb1e3)
2. [Static library](https://medium.com/@bdov_/https-medium-com-bdov-c-static-libraries-what-why-and-how-b6b442b054d3)
3. [Dynamic library, Adv and Dis-adv of static and dynamic](https://medium.com/@luischaparroc/https-medium-com-luischaparroc-dynamic-libraries-in-c-96a989848476)
4. [Dynamic library](https://medium.com/@bdov_/https-medium-com-bdov-c-dynamic-libraries-what-why-and-how-66cf777019a7)
