.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

MORE_CFLAGS = -I$(INCDIR)/$(STD)

OBJS = \
	expr.o\
	ins.o\
	main.o\
	parser.o\
	symbol.o\

TARGET =\
	as-amd64\
	as-i286\
	as-i386\
	as-powerpc\
	as-powerpc64\
	as-z80 \

MORE_LDLIBS =  -lscc -lmach

all: $(TARGET)

$(TARGET): $(LIBSCC) $(LIBMACH)

scc-as: as
	cp as $(BINDIR)

genhash.o: ../../libscc/genhash.c
	$(HOSTCC) -c ../../libscc/genhash.c

lexh.o: lexh.c
	$(HOSTCC) -c lexh.c

lexh: lexh.o genhash.o
	$(HOSTCC) -o $@ lexh.o genhash.o

clean:
	rm -f */*.o */*tbl.c lexh
	rm -f as

include powerpc/powerpc64.mk
include powerpc/powerpc.mk
include x86/amd64.mk
include x86/i386.mk
include x86/i286.mk
include x80/z80.mk
