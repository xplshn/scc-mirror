OBJ-z80-scc = $(OBJS)  \
        z80-scc/cgen.o \
        z80-scc/optm.o \
        z80-scc/code.o \
        z80-scc/types.o \

cc2-z80-scc: $(LIBSCC) $(OBJ-z80-scc)
	$(CC) $(PROJ_LDFLAGS) $(OBJ-z80-scc) -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
