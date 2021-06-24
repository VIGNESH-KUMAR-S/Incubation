# Build process
- **Build Process = `Compilation Process + Linking Process`**.

![](https://secureservercdn.net/160.153.138.219/b79.d22.myftpupload.com/wp-content/uploads/2017/08/c-compilation-process.png)

1. Preprocessing: **sample.c  to  sample.i**

- Expand the header file.
- Macro expansion.
- Remove comment lines.
- Gives expanded source code.

2. Compiler: **sample.i  to  sample.s**

- If there is no error, then the expanded source code will be converted into assembly code.
- Assembly code means mnemonics instructions.

3. Assembler: **sample.s  to  sample.o**

- Assembler used to convert assembly language into machine lenguage.
- Machine language means 0's and 1's.
- It creates object file.

4. Linker: **sample.o  to  sample.exe**

- Linker is the responsible to link all function calls and variables with respective definitions.
- Linker converts machine language to executable code.
- Linker links the static library with object file.
- It's also called **Run Time Library**.
