.POSIX:

PROJECTDIR = ../../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS_Z80 =\
	z80tbl.o\
	arch_z80.o\
	ins.o\

all: z80.o

z80.o: $(OBJS_Z80)
	$(LD) -r -o $@ $(OBJS_Z80)

z80tbl.o: ops.dat opers.dat
	../mktbl -c z80
	$(CC) $(PROJ_CFLAGS) -c z80tbl.c

clean:
	rm -f z80tbl.c
