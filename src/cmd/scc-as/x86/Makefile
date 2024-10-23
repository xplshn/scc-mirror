.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS_286 =\
	i286tbl.o\
	arch_i286.o\
	ins.o\

OBJS_386 =\
	i386tbl.o\
	arch_i386.o\
	ins.o\

OBJS_AMD64 =\
	amd64tbl.o\
	arch_amd64.o\
	ins.o\

all: i286.o i386.o amd64.o

i286.o: $(OBJS_286)
	$(LD) -r -o $@ $(OBJS_286)

i386.o: $(OBJS_386)
	$(LD) -r -o $@ $(OBJS_386)

amd64.o: $(OBJS_AMD64)
	$(LD) -r -o $@ $(OBJS_AMD64)

i286tbl.c: ops.dat opers.dat
	../mktbl -c i286

i386tbl.c: ops.dat opers.dat
	../mktbl -c i386

amd64tbl.c: ops.dat opers.dat
	../mktbl -c amd64

clean:
	rm -f amd64tbl.c i386tbl.c i286tbl.c
