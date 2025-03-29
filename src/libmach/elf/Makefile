.POSIX:

PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	elfarchs.o\
	elfdel.o\
	elfgetsec.o\
	elfgetsym.o\
	elfloadmap.o\
	elfnew.o\
	elfprobe.o\
	elfread.o\
	elftype.o\

all: $(OBJS)

dep:
	@{ printf 'ELFOBJS=\\\n';\
	printf '\telf/%s\\\n' $(OBJS); } > deps.mk

distclean:
	rm -f deps.mk
