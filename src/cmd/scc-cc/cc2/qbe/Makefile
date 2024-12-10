.POSIX:

PROJECTDIR = ../../../../..
include $(PROJECTDIR)/scripts/rules.mk


OBJS =\
	cgen.o \
	code.o \
	optm.o \
	peep.o \

all: builtin.o

builtin.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)
