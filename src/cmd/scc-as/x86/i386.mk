I386_OBJ =\
	$(OBJS)\
	x86/i386tbl.o\
	x86/i386.o\
	x86/ins.o\

x86/i386tbl.c: x86/ops.dat x86/opers.dat lexh
	./mktbl -f x86 -c i386

as-i386: $(I386_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(I386_OBJ) $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
