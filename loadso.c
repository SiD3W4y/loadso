#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>
#include <sys/mman.h>

#include "loadso.h"
#include "tracer.h"
#include "elfutils.h"

static void *__map_file(char *path)
{
    int fd = open(path , O_RDONLY);
    struct stat statbuf;

    if(fd < 0)
        return NULL;

    fstat(fd, &statbuf);

    return mmap(0, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
}

static long __module_base(int pid, char *name)
{
    FILE *fp;
    char buff[512];

    if(pid == 0) {
        fp = fopen("/proc/self/maps", "r");
    } else {
        snprintf(buff, 512, "/proc/%u/maps", pid);
        fp = fopen(buff, "r");
    }

    if(fp == NULL)
        return -1;

    char *line;

    while((line = fgets(buff, 512, fp)) != NULL) {
        if(strstr(line, name) > 0) {
            return strtoul(line, NULL, 16);
        }
    }

    return 0;
}

static char *__module_name(int pid, char *pattern)
{
    FILE *fp;
    char buff[512];

    if(pid == 0) {
        fp = fopen("/proc/self/maps", "r");
    } else {
        snprintf(buff, 512, "/proc/%u/maps", pid);
        fp = fopen(buff, "r");
    }

    if(fp == NULL)
        return NULL;

    char *line;

    while((line = fgets(buff, 512, fp)) != NULL) {
        if(strstr(line, pattern) > 0) {
            int i = strlen(line);
            line[i-1] = '\0';
            
            while(i > 0 && line[i] != ' ') {
                i--;
            }
            
            return strdup(&line[i+1]);
        }
    }

    return NULL;
}

static int __module_bits(void *mapped_elf)
{
    Elf64_Ehdr *header = (Elf64_Ehdr *)mapped_elf;

    switch(header->e_ident[EI_CLASS]) {
        case ELFCLASS32:
            return 32;
            break;
        case ELFCLASS64:
            return 64;
            break;
        case ELFCLASSNONE:
        default:
            return 0;
    }
}

loadso_err loadso_inject(int pid, char *path)
{
    long handle = tracer_attach(pid);

    if(handle < 0)
        return loadso_err_attach;

    struct stat statbuf;

    if(stat(path, &statbuf) < 0)
        return loadso_err_file;
    
    printf("[+] Injecting object \"%s\" in target process %d\n", path, pid);
    
    // Now loading the right module in memory
    char *module_name = __module_name(pid, TARGET_PATTERN);

    if(module_name == NULL)
        return loadso_err_file; // loadso_err_module

    printf("[+] Using function \"" TARGET_SYMBOL "\" from lib %s\n", module_name);
    void *module_data = __map_file(module_name);

    int bits = __module_bits(module_data);
    
    if(bits == 0)
        return loadso_err_file; // loadso_err_arch

    // now that we have the bits we can get the right function offset
    // TODO: Needs error checking
    long module_base = __module_base(pid, module_name);
    long target_offset = 0;

    if(bits == 32) {
        target_offset = elf32_getsymbol(module_data, TARGET_SYMBOL);    
    } else {        
        target_offset = elf64_getsymbol(module_data, TARGET_SYMBOL);
    }

    printf("[+] Target function at offset 0x%lx\n", target_offset + module_base);

    tracer_detach(pid);
    
    return loadso_err_ok; 
}

char *loadso_strerr(loadso_err err)
{
    switch(err) {
        case loadso_err_ptrace:
            return "Ptrace error";
            break;
        case loadso_err_attach:
            return "Could not attach to remote process";
            break;
        case loadso_err_file:
            return "Input file does not exist";
            break;
        default:
            return "No errors";
    }
}
