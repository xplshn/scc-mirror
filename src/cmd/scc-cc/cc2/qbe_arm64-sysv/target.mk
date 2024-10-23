OBJ-qbe_arm64-sysv = $(OBJS)  \
        qbe/cgen.o \
        qbe/optm.o \
        qbe/code.o \
        arm64-sysv/types.o \

cc2-qbe_arm64-sysv: $(LIBSCC) $(OBJ-qbe_arm64-sysv)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-qbe_arm64-sysv) -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
