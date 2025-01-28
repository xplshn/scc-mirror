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
	@(printf 'ELFOBJS=\\';\
	echo;\
	echo $(OBJS) |\
	sed -E 's@ *([^ ]*)@\telf/\1\\\n@g') > deps.mk

distclean:
	rm -f deps.mk
