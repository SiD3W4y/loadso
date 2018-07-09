#include <stdlib.h>
#include <sys/ptrace.h>
#include "tracer.h"

long tracer_attach(int pid)
{
    return ptrace(PTRACE_ATTACH, pid, NULL, NULL);
}

long tracer_detach(int pid)
{
    return ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

long tracer_write(int pid, long address, void *data, int size)
{
    // Warning: For the sake of simplicity this functions will not handle sizes
    //          that are not multiples of native pointer sizes
    int res;
    long *buff = data;

    for(int i=0; i < (size / sizeof(long)); i++) {
        res = ptrace(PTRACE_POKETEXT, pid, address + i * sizeof(long), buff[i]);

        if(res < 0)
            return res;
    }

    return size;
}

long tracer_read(int pid, long address, void *data, int size)
{
    // Warning: For the sake of simplicity this functions will not handle sizes
    //          that are not multiples of native pointer sizes
    int res;
    long *buff = data;

    for(int i=0; i < (size / sizeof(long)); i++) {
        res = ptrace(PTRACE_PEEKTEXT, pid, address + i * sizeof(long), NULL);
        buff[i] = res;
    }

    return size;
}
