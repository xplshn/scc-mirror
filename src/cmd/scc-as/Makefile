.POSIX:

DIRS =\
	ppc\
	x80\
	x86\

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = \
	expr.o\
	ins.o\
	main.o\
	parser.o\
	symbol.o\

TARGET =\
	scc-as\
	as-amd64\
	as-i286\
	as-i386\
	as-ppc\
	as-ppc64\
	as-z80\

MORE_LDLIBS =  -lscc -lmach

all:
	+@$(MAKE) $(DIRS)
	+@$(MAKE) $(TARGET)
	@cp as-* $(LIBEXEC)/scc
	@cp scc-as $(BINDIR)

$(DIRS): lexh
$(TARGET): $(LIBSCC) $(LIBMACH)

genhash.o: ../../libscc/genhash.c
	$(HOSTCC) -c ../../libscc/genhash.c

lexh.o: lexh.c
	$(HOSTCC) -c lexh.c

lexh: lexh.o genhash.o
	$(HOSTCC) -o $@ lexh.o genhash.o

as-ppc: $(OBJS) ppc/ppc.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) ppc/ppc.o $(PROJ_LDLIBS) -o $@

as-ppc64: $(OBJS) ppc/ppc64.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) ppc/ppc64.o $(PROJ_LDLIBS) -o $@

as-z80: $(OBJS) x80/z80.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) x80/z80.o $(PROJ_LDLIBS) -o $@

as-i286: $(OBJS) x86/i286.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) x86/i286.o $(PROJ_LDLIBS) -o $@

as-i386: $(OBJS) x86/i386.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) x86/i386.o $(PROJ_LDLIBS) -o $@

as-amd64: $(OBJS) x86/amd64.o
	$(CC) $(PROJ_LDFLAGS) $(OBJS) x86/amd64.o $(PROJ_LDLIBS) -o $@

clean:
	rm -f lexh
