#include <string.h>
#include <stdio.h>
#include <elf.h>

#include "elfutils.h"

Elf64_Shdr *elf64_getsection(void *elfbase, char *name)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *)elfbase;
    char *base = elfbase;

    // no section string table
    if(header->e_shstrndx == 0)
        return NULL;
    
    Elf64_Shdr *sections = (Elf64_Shdr *)&base[header->e_shoff];
    Elf64_Shdr *strtab = (Elf64_Shdr *)&sections[header->e_shstrndx];
    char *section_name;

    for(int i=0; i < header->e_shnum; i++) {
        section_name = (char *)&base[sections[i].sh_name + strtab->sh_offset];
        
        if(strcmp(section_name, name) == 0)
            return &sections[i];
    }

    return NULL;
}

Elf32_Shdr *elf32_getsection(void *elfbase, char *name)
{
    return NULL;
}

unsigned long elf64_getsymbol(void *elfbase, char *name)
{
    return 0;
}

unsigned long elf32_getsymbol(void *elfbase, char *name)
{
    return 0;
}
