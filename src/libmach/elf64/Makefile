.POSIX:

PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	elf64.o\
	elf64archs.o\
	elf64del.o\
	elf64getsec.o\
	elf64getsym.o\
	elf64new.o\
	elf64probe.o\
	elf64read.o\
	elf64type.o\

ELFHDRS =\
	$(INCDIR)/bits/scc/elf/elfent.h\
	$(INCDIR)/bits/scc/elf/elfhdr.h\
	$(INCDIR)/bits/scc/elf/elfphdr.h\
	$(INCDIR)/bits/scc/elf/elfshdr.h\
	$(INCDIR)/bits/scc/elf/elftypes.h\

all: $(OBJS)

$(OBJS): $(ELFHDRS)

dep:
	@(printf 'ELF64OBJS=\\';\
	echo;\
	echo $(OBJS) |\
	sed -E 's@ *([^ ]*)@\telf64/\1\\\n@g') > deps.mk

distclean:
	rm -f deps.mk
