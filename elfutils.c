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
    Elf32_Ehdr *header = (Elf32_Ehdr *)elfbase;
    char *base = elfbase;

    // no section string table
    if(header->e_shstrndx == 0)
        return NULL;
    
    Elf32_Shdr *sections = (Elf32_Shdr *)&base[header->e_shoff];
    Elf32_Shdr *strtab = (Elf32_Shdr *)&sections[header->e_shstrndx];
    char *section_name;

    for(int i=0; i < header->e_shnum; i++) {
        section_name = (char *)&base[sections[i].sh_name + strtab->sh_offset];
        
        if(strcmp(section_name, name) == 0)
            return &sections[i];
    }

    return NULL;}

unsigned long elf64_getsymbol(void *elfbase, char *name)
{
    Elf64_Shdr *strtab = elf64_getsection(elfbase, ".strtab");
    Elf64_Shdr *symtab = elf64_getsection(elfbase, ".symtab");

    if(strtab == NULL || symtab == NULL)
        return 0;
    
    Elf64_Sym *symbols_array = (Elf64_Sym *)&elfbase[symtab->sh_offset];
    char *symbol_name;

    for(int i = 0; i < symtab->sh_size / symtab->sh_entsize; i++) {
        symbol_name = (char *)&elfbase[strtab->sh_offset + symbols_array[i].st_name];
        
        if(strcmp(symbol_name, name) == 0) {
            return symbols_array[i].st_value;
        }
    }
 
    return 0;    
}

unsigned long elf32_getsymbol(void *elfbase, char *name)
{
    Elf32_Shdr *strtab = elf32_getsection(elfbase, ".strtab");
    Elf32_Shdr *symtab = elf32_getsection(elfbase, ".symtab");

    if(strtab == NULL || symtab == NULL)
        return 0;
    
    Elf32_Sym *symbols_array = (Elf32_Sym *)&elfbase[symtab->sh_offset];
    char *symbol_name;

    for(int i = 0; i < symtab->sh_size / symtab->sh_entsize; i++) {
        symbol_name = (char *)&elfbase[strtab->sh_offset + symbols_array[i].st_name];
        
        if(strcmp(symbol_name, name) == 0) {
            return symbols_array[i].st_value;
        }
    }
 
    return 0; 
}
