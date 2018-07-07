#ifndef LOADSO_H
#define LOADSO_H

// add the ifndefs for architectures
#define TARGET_LIB "/system/lib64/libc.so"

typedef enum loadso_error {
    loadso_err_ok,
    loadso_err_attach,
    loadso_err_ptrace,
    loadso_err_file
} loadso_err;

loadso_err loadso_inject(int pid, char *path);
char *loadso_strerr(loadso_err error);

#endif
