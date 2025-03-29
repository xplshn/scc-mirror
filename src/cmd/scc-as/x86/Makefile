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

i286tbl.o: ops.dat opers.dat
	../mktbl -c i286
	$(CC) $(PROJ_CFLAGS) -c i286tbl.c

i386tbl.o: ops.dat opers.dat
	../mktbl -c i386
	$(CC) $(PROJ_CFLAGS) -c i386tbl.c

amd64tbl.o: ops.dat opers.dat
	../mktbl -c amd64
	$(CC) $(PROJ_CFLAGS) -c amd64tbl.c

clean:
	rm -f amd64tbl.c i386tbl.c i286tbl.c
