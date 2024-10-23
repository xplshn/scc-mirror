I286_OBJ =\
	$(OBJS)\
	x86/i286tbl.o\
	x86/i286.o\
	x86/ins.o\

x86/i286tbl.c: x86/ops.dat x86/opers.dat lexh
	./mktbl -f x86 -c i286

as-i286: $(I286_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(I286_OBJ) $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
