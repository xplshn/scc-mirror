OBJ-amd64-sysv = $(OBJS)  \
        amd64-sysv/cgen.o \
        amd64-sysv/optm.o \
        amd64-sysv/code.o \
        amd64-sysv/types.o

cc2-amd64-sysv: $(LIBSCC) $(OBJ-amd64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-amd64-sysv) -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
