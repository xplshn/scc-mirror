.POSIX:

PROJECTDIR = ../../../../..
include $(PROJECTDIR)/scripts/rules.mk


OBJS =\
	cgen.o \
	optm.o \
	code.o \

all: builtin.o

builtin.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)
