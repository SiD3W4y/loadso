#ifndef ELF_H
#define ELF_H

#include <elf.h>

Elf64_Shdr *elf64_getsection(void *elfbase, char *name);
Elf32_Shdr *elf32_getsection(void *elfbase, char *name);

unsigned long elf64_getsymbol(void *elfbase, char *name);
unsigned long elf32_getsymbol(void *elfbase, char *name);

#endif
