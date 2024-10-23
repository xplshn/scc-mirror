Z80_OBJ =\
	$(OBJS)\
	x80/z80tbl.o\
	x80/z80.o\
	x80/ins.o\

x80/z80tbl.c: x80/ops.dat x80/opers.dat lexh
	./mktbl -f x80 -c z80

as-z80: $(OBJ) $(Z80_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(Z80_OBJ) $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
