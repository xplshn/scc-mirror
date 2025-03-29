.POSIX:

PROJECTDIR = ../../../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	cgen.o \
	code.o \
	mach.o \
	stubs.o \

all: builtin.o

builtin.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)
