AMD64_OBJ =\
	$(OBJS)\
	x86/amd64tbl.o\
	x86/amd64.o\
	x86/ins.o\

x86/amd64tbl.c: x86/ops.dat x86/opers.dat lexh
	./mktbl -f x86 -c amd64

as-amd64: $(AMD64_OBJ)
	$(CC) $(PROJ_LDFLAGS) $(AMD64_OBJ) $(PROJ_LDLIBS) -o $@
	cp $@ $(LIBEXEC)/scc
