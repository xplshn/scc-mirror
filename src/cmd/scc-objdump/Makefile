.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	main.o\
	coff32.o\
	elf.o\

TARGET = scc-objdump
MORE_LDLIBS = -lmach -lscc

all: $(TARGET)

scc-objdump: $(OBJS) $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) $(OBJS) $(PROJ_LDLIBS) -o $@
	cp $@ $(BINDIR)
