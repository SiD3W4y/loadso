#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "loadso.h"

const char *usage = "usage: \n"
                    "\tloadso <pid> <target .so library>\n";

int main(int argc, char **argv)
{
    if(argc != 3) {
        printf("%s", usage);
        return -1;
    }
    
    loadso_err err = loadso_inject(atoi(argv[1]), argv[2]);

    if(err != loadso_err_ok) {
        printf("%s\n", loadso_strerr(err));
    }

    return 0;
}
