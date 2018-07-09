# Customize flags as you wish
CFLAGS= -std=gnu99 -g -Wall

# Code shared between library and executable
SHARED_SRC=loadso.c tracer.c elfutils.c
SHARED_OBJ=$(patsubst %.c, %.o, $(SHARED_SRC))

# Code only for the executable
INJECTOR_SRC=main.c
INJECTOR_OBJ=$(patsubst %.c, %.o, $(INJECTOR_SRC))

# Makefile logic
all: loadso loadso.so

loadso: $(SHARED_OBJ) $(INJECTOR_OBJ)
	$(CC) $(SHARED_OBJ) $(INJECTOR_OBJ) -o $@

loadso.so: $(SHARED_OBJ)
	$(CC) $(SHARED_OBJ) -shared -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(SHARED_OBJ)
	rm -rf $(INJECTOR_OBJ)
	rm -rf loadso
	rm -rf loadso.so
