# loadso
loadso is a shared library injector for linux and android, it is barebones and contains
only what is necessary to inject an object in another process memory

## Project status
The project is a WIP, for now only the resolution of dlopen functions in 32 and 64bits linux/android works.
Shellcodes for each arch will need to be created and code cleaning will be needed on error handling.

## Process
The injector will first find the module containing a hidden implementation of dlopen (libc.so on linux
and linker/linker64 on android). It will then call the function in a shellcode injected in the process using
ptrace.
