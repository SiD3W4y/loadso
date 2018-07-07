#ifndef TRACER_H
#define TRACER_H

long tracer_attach(int pid); 
long tracer_detach(int pid);
long tracer_write(int pid , long address, void *data, int size);
long tracer_read(int pid, long address, void *data, int size);

#endif
