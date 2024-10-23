.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS32 =\
	ppctbl.o\
	arch_ppc.o\
	ins.o\

OBJS64 =\
	ppc64tbl.o\
	arch_ppc64.o\
	ins.o\

all: ppc.o ppc64.o

ppc.o: $(OBJS32)
	$(LD) -r -o $@ $(OBJS32)

ppc64.o: $(OBJS64)
	$(LD) -r -o $@ $(OBJS64)

ppctbl.c: ops.dat opers.dat
	../mktbl -c ppc

ppc64tbl.c: ops.dat opers.dat
	../mktbl -c ppc64

clean:
	rm -f ppc64tbl.c ppctbl.c
