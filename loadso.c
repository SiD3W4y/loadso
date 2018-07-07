#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ptrace.h>

#include "loadso.h"
#include "tracer.h"

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

loadso_err loadso_inject(int pid, char *path)
{
    long handle = tracer_attach(pid);

    if(handle < 0)
        return loadso_err_attach;

    struct stat statbuf;

    if(stat(path, &statbuf) < 0)
        return loadso_err_file;
    
    printf("[+] Injecting object \"%s\" in target process %d\n", path, pid);
    
    // We compute relative offsets for the functions
    long libc_local_base = __module_base(0, TARGET_LIB);
    long libc_remote_base = __module_base(pid, TARGET_LIB);

    long dlopen_offset = (long)dlopen - libc_local_base;
    long remote_dlopen = libc_remote_base + dlopen_offset;

    printf("[+] Local libc: 0x%lx, Remote libc: 0x%lx\n", libc_local_base, libc_remote_base);
    printf("[+] Remote dlopen: 0x%lx\n", remote_dlopen);

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
