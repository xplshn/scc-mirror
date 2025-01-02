.POSIX:

PROJECTDIR = ../../../../..
include $(PROJECTDIR)/scripts/rules.mk


OBJS =\
	cgen.o \
	code.o \
	mach.o \
	peep.o \
	types.o \

all: builtin.o

builtin.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)
