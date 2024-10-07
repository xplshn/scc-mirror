.POSIX:

DIRS   =\
	ld\
	as\
	cc\
	make\

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	scc-nm\
	scc-ar\
	scc-strip\
	scc-size\
	scc-ranlib\
	scc-objdump\
	scc-objcopy\
	scc-addr2line\
	qbe/qbe\

all: $(TARGET) $(DIRS)

qbe/qbe: qbe
	cp $@ $(LIBEXEC)/scc

qbe/.git:
	git submodule init qbe
	git submodule update qbe

qbe: qbe/.git FORCE
	+cd qbe && $(MAKE) CC=$(CC) CFLAGS='$(PROJ_CFLAGS)'

scc-nm: nm.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) nm.o -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-strip: strip.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) strip.o -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-size: size.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) size.o -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-ranlib: ranlib.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ranlib.o -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-objdump: objdump.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objdump.o -lmach $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-objcopy: objcopy.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objcopy.o -lmach $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-addr2line: addr2line.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) addr2line.o -lmach -lscc $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)

scc-ar: ar.o $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ar.o $(PROJ_LDLIBS) -lscc -o $@
	cp $@ $(BINDIR)

include deps.mk
