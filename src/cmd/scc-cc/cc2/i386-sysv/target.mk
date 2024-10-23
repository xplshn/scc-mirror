OBJ-i386-sysv = $(OBJS)  \
        i386-sysv/cgen.o \
        i386-sysv/optm.o \
        i386-sysv/code.o \
        i386-sysv/types.o

cc2-i386-sysv: $(LIBSCC) $(OBJ-i386-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-i386-sysv) -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
