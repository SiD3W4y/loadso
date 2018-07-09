#ifndef ELF_H
#define ELF_H

#include <elf.h>

#ifdef __ANDROID_API__
    #ifdef __aarch64__
        #define TARGET_PATTERN "/system/bin/linker64"
        #define TARGET_SYMBOL "__dl_dlopen"
    #else
        #define TARGET_PATTERN "/system/bin/linker"
        #define TARGET_SYMBOL "__dl_dlopen"
    #endif
#else
    #ifdef __x86_64__
        #define TARGET_PATTERN "libc-"
        #define TARGET_SYMBOL "__libc_dlopen_mode"
    #else
        #define TARGET_PATTERN "libc-"
        #define TARGET_SYMBOL "__libc_dlopen_mode"
    #endif
#endif


Elf64_Shdr *elf64_getsection(void *elfbase, char *name);
Elf32_Shdr *elf32_getsection(void *elfbase, char *name);

unsigned long elf64_getsymbol(void *elfbase, char *name);
unsigned long elf32_getsymbol(void *elfbase, char *name);

#endif
