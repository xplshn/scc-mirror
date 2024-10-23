OBJ-qbe_amd64-sysv = $(OBJS)  \
        qbe/cgen.o \
        qbe/optm.o \
        qbe/code.o \
        amd64-sysv/types.o

cc2-qbe_amd64-sysv: $(LIBSCC) $(OBJ-qbe_amd64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-qbe_amd64-sysv) -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
