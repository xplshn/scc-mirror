.POSIX:

PROJECTDIR = ../../../../..
include $(PROJECTDIR)/scripts/rules.mk


OBJS =\
	../amd64-sysv/types.o\
	../qbe/builtin.o\

all: builtin.o

builtin.o: $(OBJS)
	$(LD) -r -o $@ $(OBJS)
